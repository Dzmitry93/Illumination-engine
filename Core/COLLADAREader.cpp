#include "COLLADAReader.h"

namespace OGE
{
	namespace fileSystem
	{
		string COLLADAReader::defaultString = "";
		uint32 COLLADAReader::defaultUint = 0;

		string COLLADAReader::vertexSemantic = "VERTEX";
		string COLLADAReader::normalSemantic = "NORMAL";
		string COLLADAReader::texcoordSemantic = "TEXCOORD";

		COLLADADocument* COLLADAReader::loadScene(const string& rPath)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(rPath.data());

			if(result.status != pugi::xml_parse_status::status_ok)
			{
				error_msg("Invalid COLLADA file");
				return NULL;
			}

			pugi::xml_node colladaDoc = doc.child("COLLADA");
			COLLADADocument* pColladaDoc = new COLLADADocument;

			// parse geometry library
			pugi::xml_node libraryGeometriesNode = colladaDoc.child("library_geometries");
			pColladaDoc->pLibraryGeometries = loadLibraryGeometry(libraryGeometriesNode);

			// parse effects library 
			pugi::xml_node libraryEffectsNode = colladaDoc.child("library_effects");
			pColladaDoc->pLibraryEffects = loadLibraryEffects(libraryEffectsNode);

			//parse materials 
			pugi::xml_node libraryMaterialNode = colladaDoc.child("library_materials");
			pColladaDoc->pLibraryMaterials = loadLibraryMaterial(libraryMaterialNode);

			// parse images
			pugi::xml_node libraryImagesNode = colladaDoc.child("library_images");
			pColladaDoc->pLibraryImages = loadLibraryImages(libraryImagesNode);

			// parse lights
			pugi::xml_node libraryLightsNode = colladaDoc.child("library_lights");
			pColladaDoc->pLibraryLights = loadLibraryLights(libraryLightsNode);

			// parse cameras 
			pugi::xml_node libraryCamerasNode = colladaDoc.child("library_cameras");
			pColladaDoc->pLibraryCameras = loadLibraryCameras(libraryCamerasNode);

			// parse scene
			pugi::xml_node libraryVisualScenesNode = colladaDoc.child("library_visual_scenes");
			pColladaDoc->pLibraryVisualScenes = loadLibraryVisualScenes(libraryVisualScenesNode);

			return pColladaDoc;
		}

		CLibraryGeometries* COLLADAReader::loadLibraryGeometry(pugi::xml_node& rLibraryNode)
		{
			CLibraryGeometries* pLibraryGeomtries = new CLibraryGeometries();
			CGeometry* pGeometry = NULL;
			
			// parse geometry node
			List<CGeometry> listGeometry;
			for(pugi::xml_node geometryNode = rLibraryNode.child("geometry"); 
				geometryNode; geometryNode = geometryNode.next_sibling("geometry"))
			{
				CGeometry geometry;
				CMesh mesh;

				geometry.id = geometryNode.attribute("id").as_string();
				geometry.name = geometryNode.attribute("name").as_string();
				geometry.mesh = mesh;

				// parse mesh
				pugi::xml_node meshNode = geometryNode.child("mesh");
				if(!meshNode.empty()) 
				{
					// parse sources
					parseSources(meshNode, &mesh.pSources, mesh.sourcesCount);

					// parse vertices node
					parseVertices(meshNode, mesh.vertices);

					// parse polygons node
					parsePolygons(meshNode,
						&mesh.pPrimitivesBuffers, mesh.countPrimitivesBuffers); 
				}

				geometry.mesh = mesh;
				listGeometry.push_back(geometry);
			}

			parseListData<CGeometry>(listGeometry,
				&pLibraryGeomtries->pGeometry, 
				pLibraryGeomtries->countGeometries);

			return pLibraryGeomtries;
		}

		CLibraryMaterial* COLLADAReader::loadLibraryMaterial(pugi::xml_node& rLibraryNode)
		{
			CLibraryMaterial* pLibraryMaterials = new CLibraryMaterial();
			
			List<CMaterial> materialsList;
			CMaterial material;

			for(pugi::xml_node materialNode = rLibraryNode.child("material"); 
				materialNode; materialNode = materialNode.next_sibling("material"))
			{
				// parse material node
				parseMaterial(materialNode, material);
				materialsList.push_back(material);
			}

			// set materials library data
			parseListData<CMaterial>(materialsList,
				&pLibraryMaterials->pMaterials,
				pLibraryMaterials->countMaterials);

			return pLibraryMaterials;
		}

		CLibraryEffects* COLLADAReader::loadLibraryEffects(pugi::xml_node& rLibraryNode)
		{
			CLibraryEffects* pLibraryEffects = new CLibraryEffects();
			
			List<CEffect> effectsList;
			for(pugi::xml_node effectNode = rLibraryNode.child("effect"); 
				effectNode; effectNode = effectNode.next_sibling("effect"))
			{
				CEffect effect;
				CEffectProfileCommon profileCommon;
				CEffectTechnique technique;
				CEffectMaterial* pPhong = new CEffectMaterial();

				effect.id = effectNode.attribute("id").as_string();
				effect.name = effectNode.attribute("name").as_string();

				// parse technique
				pugi::xml_node techniqueNode = effectNode.child("profile_COMMON").child("technique");
				technique.sid = techniqueNode.attribute("sid").as_string();

				pugi::xml_node phongNode = techniqueNode.child("phong");
				
				//parse phong node
				if(!phongNode.empty())
				{
					parseEffectMaterial(phongNode, *pPhong);
				}

				// set effect data
				technique.pEffectMaterial = pPhong;
				profileCommon.technique = technique;
				effect.profileCommon = profileCommon;
				
				effectsList.push_back(effect);
			}

			// set effects library data
			parseListData<CEffect>(effectsList, 
				&pLibraryEffects->pEffect, 
				pLibraryEffects->countEffects);

			return pLibraryEffects;
		}

		CLibraryImages* COLLADAReader::loadLibraryImages(pugi::xml_node& rLibraryNode)
		{
			CLibraryImages* pLibraryImages = new CLibraryImages();
			pugi::xml_node imagesLibraryNode = rLibraryNode.child("library_images");

			List<CImage> imagesList;
			for(pugi::xml_node imageNode = rLibraryNode.child("image");
				imageNode; imageNode = imageNode.next_sibling("image"))
			{
				CImage image;
				parseImage(imageNode, image);

				imagesList.push_back(image);
			}

			parseListData(imagesList, 
				&pLibraryImages->pImages,
				pLibraryImages->countImages);

			return pLibraryImages;
		}

		CLibraryVisualScenes* COLLADAReader::loadLibraryVisualScenes(pugi::xml_node& rLibraryNode)
		{
			CLibraryVisualScenes* pLibraryScenes = new CLibraryVisualScenes();
			CVisualScene* pScenes = NULL;

			List<CVisualScene> scenesList;
			for(pugi::xml_node sceneNode = rLibraryNode.child("visual_scene"); 
				sceneNode; sceneNode = sceneNode.next_sibling("visual_scene"))
			{
				CVisualScene visualScene;
				CNode** ppNodes = NULL;

				List<CNode*> nodesList;
				for(pugi::xml_node node = sceneNode.child("node"); 
					node; node = node.next_sibling("node"))
				{
					// parse node
					CNode* nodeObj = new CNode();
					parseNode(node, nodeObj);

					nodesList.push_back(nodeObj);
				}

				// set scene data
				visualScene.id = sceneNode.attribute("id").as_string();
				visualScene.name = sceneNode.attribute("name").as_string();

				uint32 nodeIndex = 0;
				visualScene.countNodes = nodesList.size();
				ppNodes = new CNode*[visualScene.countNodes];

				ListIterator<CNode*> it = nodesList.iterator();

				while(it.hasNext())
				{
					ppNodes[nodeIndex++] = it.next();
				}

				visualScene.ppNodes = ppNodes;
				scenesList.push_back(visualScene);
			}

			parseListData<CVisualScene>(scenesList, 
				&pLibraryScenes->pScenes, 
				pLibraryScenes->countScenes);

			return pLibraryScenes;
		}

		CLibraryLights* COLLADAReader::loadLibraryLights(pugi::xml_node& rLibraryNode)
		{
			CLibraryLights* pLibraryLights = new CLibraryLights;

			List<CSpotlight> spotlights;
			List<CDirectionalLight> directionalLights;
			List<CPointLight> pointLights;

			CSpotlight spotlight;
			CPointLight pointLight;
			CDirectionalLight directionalLight;

			for(pugi::xml_node lightNode = rLibraryNode.child("light"); 
				lightNode; lightNode = lightNode.next_sibling("light"))
			{
				pugi::xml_node techniqueCommon = lightNode.child("technique_common");

				if(!techniqueCommon.child("spot").empty())
				{
					parseSpotlight(techniqueCommon.child("spot"), spotlight);
					spotlight.id = lightNode.attribute("id").as_string();
					spotlights.push_back(spotlight);
				}
				
				if(!techniqueCommon.child("point").empty())
				{
					parsePointLight(techniqueCommon.child("point"), pointLight);
					pointLight.id = lightNode.attribute("id").as_string();
					pointLights.push_back(pointLight);
				}

				if(!techniqueCommon.child("directional").empty())
				{
					parseDirectionalLight(techniqueCommon.child("directional"), directionalLight);
					directionalLight.id = lightNode.attribute("id").as_string();
					directionalLights.push_back(directionalLight);
				}
			}

			parseListData(spotlights, &pLibraryLights->pSpotlights, pLibraryLights->countSpotlights);
			parseListData(directionalLights, &pLibraryLights->pDirectionalLights, pLibraryLights->countDirectionalLights);
			parseListData(pointLights, &pLibraryLights->pPointLights, pLibraryLights->countPointLights);

			return pLibraryLights;
		}

		CLibraryCameras* COLLADAReader::loadLibraryCameras(pugi::xml_node& rLibraryNode)
		{
			CLibraryCameras* pLibraryCameras = new CLibraryCameras;

			List<CCamera> listCameras;
			CCamera camera;

			for(pugi::xml_node cameraNode = rLibraryNode.child("camera"); 
				cameraNode; cameraNode = cameraNode.next_sibling("camera"))
			{
				camera.id = cameraNode.attribute("id").as_string();
				
				parseOptics(cameraNode.child("optics"), camera.optics);
				listCameras.push_back(camera);	
			}

			parseListData(listCameras, &pLibraryCameras->pCameras, pLibraryCameras->countCameras);
			return pLibraryCameras;
		}
		
		void COLLADAReader::parsePolygons(pugi::xml_node& meshNode, CPolygons** ppPolygonsBuffers,
			uint32& countBuffers)
		{
			List<CPolygons> polygonsBuffersList;
			for(pugi::xml_node polygonsNode = meshNode.child("triangles");
				polygonsNode; polygonsNode = polygonsNode.next_sibling("triangles"))
			{
				CPolygons polygons;
				// read polygons input semantic
				List<CInput> inputs;
				CInput input;
				for(pugi::xml_node inputNode = polygonsNode.child("input");
					inputNode; inputNode = inputNode.next_sibling("input"))
				{
					parseInput(inputNode, input);
					inputs.push_back(input);
				}

				polygons.pInputs = NULL;
				polygons.pPrimitive = NULL;
				polygons.countPrimitives = polygonsNode.attribute("count").as_int();

				// parse faces
				CPolygon* pPolygons = new CPolygon[polygons.countPrimitives];
				uint32 polygonIndex = 0;

				pugi::xml_node polygonNode = polygonsNode.child("p");
				istringstream strStream(polygonNode.text().as_string());

				CPolygon polygon;
				while(!strStream.eof())
				{

					uint32* pData = new uint32[3];
					uint32 index = 0;

					strStream >> pData[0];
					strStream >> pData[1];
					strStream >> pData[2];
				
					polygon.count = 3;
					polygon.pData = pData;

					pPolygons[polygonIndex] = polygon;
					polygonIndex++;
				}

				// set polygons data
				parseListData<CInput>(inputs,
					&polygons.pInputs,
					polygons.countInputs);

				polygons.pPrimitive = pPolygons;
				polygons.materialId = polygonsNode.attribute("material").as_string();

				polygonsBuffersList.push_back(polygons);
			}
			
			countBuffers = polygonsBuffersList.size();
			uint32 bufferIndex = 0;
			ListIterator<CPolygons> it = polygonsBuffersList.iterator();
			*ppPolygonsBuffers = new CPolygons[countBuffers];

			while(it.hasNext())
			{
				(*ppPolygonsBuffers)[bufferIndex++] = it.next();
			}
		}

		void COLLADAReader::parseVertices(pugi::xml_node& meshNode, CVertices& vertices)
		{
			
			CInput inputSemantic;
			uint32 semanticIndex = 0;
			pugi::xml_node verticesNode = meshNode.child("vertices");

			vertices.id = verticesNode.attribute("id").as_string();
			vertices.name = verticesNode.attribute("name").as_string();
			vertices.pInputSemantic = new CInput[5];
			
			for(pugi::xml_node inputSemanticNode = verticesNode.child("input");
				inputSemanticNode; inputSemanticNode = inputSemanticNode.next_sibling("input"))
			{
				inputSemantic.semantic = inputSemanticNode.attribute("semantic").as_string();
				inputSemantic.sourceId = inputSemanticNode.attribute("source").as_string();
				inputSemantic.offset = inputSemanticNode.attribute("offset").as_int();

				vertices.pInputSemantic[semanticIndex] = inputSemantic;
				semanticIndex++;
			}

			vertices.countSemantics = semanticIndex;
		}

		void COLLADAReader::parseSources(pugi::xml_node& meshNode, CSource** ppSource, uint32& count)
		{
			CSource source;
			CFloatArray floatArray;;
			CAccessor accessor;

			List<CSource> sources;
			for(pugi::xml_node sourceNode = meshNode.child("source"); 
				sourceNode; sourceNode = sourceNode.next_sibling("source"))
			{

				// parse array data
				pugi::xml_node arrayNode = sourceNode.child("float_array");
				parseArray(arrayNode, floatArray);
				
				// parse accessor source
				pugi::xml_node accessorNode = sourceNode.child("technique_common").child("accessor");
				parseAccessor(accessorNode, accessor);

				// set source data
				source.id = sourceNode.attribute("id").as_string();
				source.name = sourceNode.attribute("name").as_string();
				source.arrayEllement = floatArray;
				source.accessor = accessor;

				sources.push_back(source);
			}

			count = sources.size();
			uint32 index = 0;
			ListIterator<CSource> it = sources.iterator();
			CSource* pSource = new CSource[count];

			while(it.hasNext())
			{
				pSource[index++] = it.next();
			}

			*ppSource = pSource;
		}

		void COLLADAReader::parseAccessor(pugi::xml_node& accessorNode, CAccessor& accessor)
		{
			List<CParam> paramsList;
			for(pugi::xml_node paramNode = accessorNode.child("param"); 
				paramNode; paramNode = paramNode.next_sibling("param"))
			{
				CParam param;
				parseParam(paramNode, param);

				paramsList.push_back(param);
			}

			parseListData<CParam>(paramsList, &accessor.pParams, accessor.count);
			
			accessor.arrayId = accessorNode.attribute("source").as_string();
			accessor.count = accessorNode.attribute("count").as_int();
			accessor.stride = accessorNode.attribute("stride").as_int();
		}

		void COLLADAReader::parseParam(pugi::xml_node& paramNode, CParam& param)
		{
			param.name = paramNode.attribute("name").as_string();
			param.type = paramNode.attribute("type").as_string();
		}

		void COLLADAReader::parseColor(pugi::xml_node& colorNode, CColor& color)
		{
			string colorValue = colorNode.text().as_string();
			istringstream stream(colorValue);
			
			stream >> color.value[0];
			stream >> color.value[1];
			stream >> color.value[2];
			stream >> color.value[3];

			color.sid = colorNode.attribute("sid").as_string();
		}

		void COLLADAReader::parseFloat(pugi::xml_node& floatNode, CFloat& value)
		{
			string floatValue = floatNode.text().as_string();
			istringstream stream(floatValue);
			
			stream >> value.value;
			value.sid = floatNode.attribute("sid").as_string();
		}

		void COLLADAReader::parseFloatArray(string& data, float* pFloatArray)
		{
			istringstream strStream(data);
			uint32 index = 0;

			while(!strStream.eof())
			{
				strStream >> pFloatArray[index];
				index++;
			}
		}

		void COLLADAReader::parseArray(pugi::xml_node& arrayNode, CFloatArray& floatArray)
		{
			floatArray.id = arrayNode.attribute("id").as_string();
			floatArray.name = arrayNode.attribute("name").as_string();
			floatArray.digits = arrayNode.attribute("digits").as_int();
			floatArray.magnitude = arrayNode.attribute("magnitude").as_int();
			floatArray.count = arrayNode.attribute("count").as_int();
					
			string arrayString = arrayNode.text().as_string();
			uint32 countElements = floatArray.count;
			float* pFloatData = new float[countElements];
			parseFloatArray(arrayString, pFloatData);

			floatArray.pData = pFloatData;
		}

		void COLLADAReader::parseMatrix(pugi::xml_node& matixNode, CMatrix& matrix)
		{
			string values = matixNode.text().as_string();
			istringstream stream(values);

			stream >> matrix.data[0][0];
			stream >> matrix.data[1][0];
			stream >> matrix.data[2][0];
			stream >> matrix.data[3][0];

			stream >> matrix.data[0][1];
			stream >> matrix.data[1][1];
			stream >> matrix.data[2][1];
			stream >> matrix.data[3][1];

			stream >> matrix.data[0][2];
			stream >> matrix.data[1][2];
			stream >> matrix.data[2][2];
			stream >> matrix.data[3][2];

			stream >> matrix.data[0][3];
			stream >> matrix.data[1][3];
			stream >> matrix.data[2][3];
			stream >> matrix.data[3][3];
		}

		void COLLADAReader::parseRotate(pugi::xml_node& rRotateNode, CRotate& rRotate)
		{
			string values = rRotateNode.text().as_string();
			istringstream stream(values);

			stream >> rRotate.x;
			stream >> rRotate.y;
			stream >> rRotate.z;
			stream >> rRotate.degree;

			rRotate.sid = rRotateNode.attribute("sid").as_string();
		}

		void COLLADAReader::parseTranslate(pugi::xml_node& rTranslateNode, CTranslate& rTranslate)
		{
			string values = rTranslateNode.text().as_string();
			istringstream stream(values);

			stream >> rTranslate.x;
			stream >> rTranslate.y;
			stream >> rTranslate.z;
		}

		void COLLADAReader::parseScale(pugi::xml_node& rScaleNode, CScale& rScale)
		{
			string values = rScaleNode.text().as_string();
			istringstream stream(values);

			stream >> rScale.x;
			stream >> rScale.y;
			stream >> rScale.z;
		}

		void COLLADAReader::parseBindMaterial(pugi::xml_node& bindMaterialNode, CBindMaterial& bindMaterial)
		{
			List<CInstanceMaterial> materialsList;
			for(pugi::xml_node instMatNode = bindMaterialNode.child("technique_common").child("instance_material");
				instMatNode; instMatNode = instMatNode.next_sibling("instance_material"))
			{
				CInstanceMaterial instMat;
				parseInstanceMaterial(instMatNode, instMat);

				materialsList.push_back(instMat);
			}

			parseListData(materialsList, &bindMaterial.pInstanceMaterials, bindMaterial.countInstances);
		}

		void COLLADAReader::parseInstanceMaterial(pugi::xml_node& instMatNode, CInstanceMaterial& instMat)
		{
			instMat.sid = instMatNode.attribute("sid").as_string();
			instMat.name = instMatNode.attribute("name").as_string();
			instMat.symbolId = instMatNode.attribute("symbol").as_string();
			instMat.targetId = instMatNode.attribute("target").as_string();
		}

		void COLLADAReader::parseInstanceGeometry(pugi::xml_node& instGeomNode,
			CInstanceGeometry& instanceGeometry)
		{
			CBindMaterial* pBindMaterial = NULL;
			pugi::xml_node bindMaterialNode = instGeomNode.child("bind_material");

			if(!bindMaterialNode.empty())
			{
				pBindMaterial = new CBindMaterial();
				parseBindMaterial(bindMaterialNode, *pBindMaterial);
			}

			instanceGeometry.geometryId = instGeomNode.attribute("url").as_string();
			instanceGeometry.name = instGeomNode.attribute("name").as_string();
			instanceGeometry.sid = instGeomNode.attribute("sid").as_string();
			instanceGeometry.pBindMaterial = pBindMaterial;
		}

		void COLLADAReader::parseInstanceCamera(pugi::xml_node& rInstanceCameraNode,
			CInstanceCamera& rInstanceCamera)
		{
			rInstanceCamera.url = rInstanceCameraNode.attribute("url").as_string();
			rInstanceCamera.sid = rInstanceCameraNode.attribute("sid").as_string();
			rInstanceCamera.name = rInstanceCameraNode.attribute("name").as_string();
		}

		void COLLADAReader::parseInstanceLight(pugi::xml_node& rInstanceLightNode,
			CInstanceLight& rInstanceLight)
		{
			rInstanceLight.url = rInstanceLightNode.attribute("url").as_string();
			rInstanceLight.sid = rInstanceLightNode.attribute("sid").as_string();
			rInstanceLight.name = rInstanceLightNode.attribute("name").as_string();
		}

		void COLLADAReader::parseNode(pugi::xml_node& node, CNode* pNode)
		{
			CMatrix* pMatrix = NULL;

			CInstanceGeometry* pInstanceGeometry = NULL;
			CInstanceCamera* pInstanceCamera = NULL;
			CInstanceLight* pInstanceLight = NULL;

			List<CRotate> rotationList;
			List<CTranslate> translationList;
			List<CScale> scaleList;

			pugi::xml_node instanceGeometryNode = node.child("instance_geometry");
			pugi::xml_node instanceLightNode = node.child("instance_light");
			pugi::xml_node instanceCameraNode = node.child("instance_camera");
			pugi::xml_node matrixNode = node.child("matrix");
			pugi::xml_node rotateNode = node.child("rotate");
			pugi::xml_node translateNode = node.child("translate");
			pugi::xml_node scaleNode = node.child("scale");

			// parse matrix
			if(!matrixNode.empty())
			{
				pMatrix = new CMatrix();
				parseMatrix(matrixNode, *pMatrix);
			}

			if(!rotateNode.empty())
			{
				for(rotateNode; rotateNode; rotateNode = rotateNode.next_sibling("rotate"))
				{
					CRotate rotate;
					parseRotate(rotateNode, rotate);
					rotationList.push_back(rotate);
				}
			}

			if(!translateNode.empty())
			{
				for(translateNode; translateNode; translateNode = translateNode.next_sibling("translate"))
				{
					CTranslate translate;
					parseTranslate(translateNode, translate);
					translationList.push_back(translate);
				}
			}

			if(!scaleNode.empty())
			{
				for(scaleNode; scaleNode; scaleNode = scaleNode.next_sibling("scale"))
				{
					CScale scale;
					parseScale(scaleNode, scale);
					scaleList.push_back(scale);
				}
			}
		
			// parse instance geometry
			if(!instanceGeometryNode.empty())
			{
				pInstanceGeometry = new CInstanceGeometry();
				parseInstanceGeometry(instanceGeometryNode, *pInstanceGeometry);
			}

			if(!instanceCameraNode.empty())
			{
				pInstanceCamera = new CInstanceCamera();
				parseInstanceCamera(instanceCameraNode, *pInstanceCamera);
			}

			if(!instanceLightNode.empty())
			{
				pInstanceLight = new CInstanceLight();
				parseInstanceLight(instanceLightNode, *pInstanceLight);
			}

			pNode->id = node.attribute("id").as_string();
			pNode->name = node.attribute("name").as_string();
			pNode->sid = node.attribute("sid").as_string();
			pNode->pInstanceGeometry = pInstanceGeometry;
			pNode->pInstanceCamera = pInstanceCamera;
			pNode->pInstanceLight = pInstanceLight;

			if(!pMatrix)
			{
				pNode->pMatrix = NULL;
				pNode->pRTS = new CNode::RTS();

				if(rotationList.size() > 0)
				{
					parseListData(rotationList, &pNode->pRTS->pRotate, pNode->pRTS->countRotate);
				}
				else
				{
					pNode->pRTS->pRotate = NULL;
					pNode->pRTS->countRotate = 0;
				}

				if(scaleList.size() > 0)
				{
					parseListData(scaleList, &pNode->pRTS->pScale, pNode->pRTS->countScale);
				}
				else
				{
					pNode->pRTS->pScale = NULL;
					pNode->pRTS->countScale = 0;
				}

				if(translationList.size() > 0)
				{
					parseListData(translationList, &pNode->pRTS->pTranslate, pNode->pRTS->countTranslate);
				}
				else
				{
					pNode->pRTS->pTranslate = NULL;
					pNode->pRTS->countTranslate = 0;
				}
			}
			else
			{
				pNode->pRTS = NULL;
				pNode->pMatrix = pMatrix;
			}

			for(pugi::xml_node internalNode = node.child("node");
				internalNode; internalNode = internalNode.next_sibling("node"))
			{
				CNode* pInternalNode = new CNode();
				parseNode(internalNode, pInternalNode);

				pNode->nodes.push_back(pInternalNode);
			}
		}

		void COLLADAReader::parseInput(pugi::xml_node& inputNode, CInput& input)
		{
			input.semantic = inputNode.attribute("semantic").as_string();
			input.offset = inputNode.attribute("offset").as_int();
			input.sourceId = inputNode.attribute("source").as_string();
		}

		void COLLADAReader::parseEffectMaterial(pugi::xml_node& phongNode,
			CEffectMaterial& phong)
		{
			parseMaterialColor(phongNode.child("emission"), phong.emmision);
			parseMaterialColor(phongNode.child("ambient"),  phong.ambient);
			parseMaterialColor(phongNode.child("diffuse"), phong.diffuse);
			parseMaterialColor(phongNode.child("transparent"), phong.trasparent);
			parseMaterialDegree(phongNode.child("transparency"), phong.transparency);
			parseMaterialColor(phongNode.child("reflective"), phong.reflective);
			parseMaterialDegree(phongNode.child("reflectivity"), phong.reflectivity);
			parseMaterialColor(phongNode.child("specular"), phong.specular);
			parseMaterialDegree(phongNode.child("shininess"), phong.shininess);
		}

		void COLLADAReader::parseMaterial(pugi::xml_node& materialNode, CMaterial& material)
		{
			material.id = materialNode.attribute("id").as_string();
			material.name = materialNode.attribute("name").as_string();
			material.effectId = materialNode.child("instance_effect").attribute("url").as_string();
		}

		void COLLADAReader::parseImage(pugi::xml_node& imageNode, CImage& image)
		{
			CInitFrom initUrl;
			
			initUrl.url = imageNode.child("init_from").text().as_string();

			image.initFromUrl = initUrl;
			image.id = imageNode.attribute("id").as_string();
			image.name = imageNode.attribute("name").as_string();
			image.sid = imageNode.attribute("sid").as_string();
		}

		void COLLADAReader::parseTexture(pugi::xml_node& textureNode, CTexture& texture)
		{
			texture.textureId = textureNode.attribute("texture").as_string();
		}

		void COLLADAReader::parseMaterialColor(pugi::xml_node& materialColortNode, 
			CMaterialColorNode& materialColor)
		{
			CColor* pColor = NULL;
			CTexture* pTexture = NULL;

			pugi::xml_node colorNode = materialColortNode.child("color");
			pugi::xml_node textureNode = materialColortNode.child("texture");

			if(!colorNode.empty())
			{
				pColor = new CColor();
				parseColor(colorNode, *pColor);
			}

			if(!textureNode.empty())
			{
				pTexture = new CTexture();
				parseTexture(textureNode, *pTexture);	
			}

			materialColor.pTexture = pTexture;
			materialColor.pColor = pColor;
		}

		void COLLADAReader::parseMaterialDegree(pugi::xml_node& materialDegreeNode,
				CMaterialDegreeNode& matDegree)
		{
			CFloat floatValue;
			pugi::xml_node floatNode = materialDegreeNode.child("float");
			parseFloat(floatNode, floatValue);

			matDegree.value = floatValue;
		}

		void COLLADAReader::parseDirectionalLight(pugi::xml_node& lightNode, CDirectionalLight& rDirLight)
		{
			pugi::xml_node colorNode = lightNode.child("color");
			parseColor(colorNode, rDirLight.lightColor);
		}

		void COLLADAReader::parsePointLight(pugi::xml_node& lightNode, CPointLight& rPointLight)
		{
			pugi::xml_node colorNode = lightNode.child("color");
			parseColor(colorNode, rPointLight.lightColor);

			pugi::xml_node constantAttenuationNode = lightNode.child("constant_attenuation");
			pugi::xml_node linearAttenuationNode = lightNode.child("linear_attenuation");
			pugi::xml_node quadraticAttenuationNode = lightNode.child("quadratic_attenuation");

			parseLightAttenuation(constantAttenuationNode, 
									linearAttenuationNode,
									quadraticAttenuationNode,
									rPointLight.attenuation);

		}

		void COLLADAReader::parseSpotlight(pugi::xml_node& lightNode, CSpotlight& rSpotlight)
		{
			pugi::xml_node colorNode = lightNode.child("color");
			parseColor(colorNode, rSpotlight.lightColor);

			pugi::xml_node constantAttenuationNode = lightNode.child("constant_attenuation");
			pugi::xml_node linearAttenuationNode = lightNode.child("linear_attenuation");
			pugi::xml_node quadraticAttenuationNode = lightNode.child("quadratic_attenuation");

			parseLightAttenuation(constantAttenuationNode, 
									linearAttenuationNode,
									quadraticAttenuationNode,
									rSpotlight.attenuation);

			// 45.0f default falloff angle
			rSpotlight.falloffAngle = lightNode.child("falloff_angle").text().as_float(45.0f);

			// 1.0f is default falloff exponent 
			rSpotlight.falloffExp = lightNode.child("falloff_exponent").text().as_float(0.0f);

		}

		void COLLADAReader::parseLightAttenuation(pugi::xml_node& constantAttenuationNode, 
										pugi::xml_node& linearAttenuationNode,
										pugi::xml_node& quadraticAttenuationNode, 
										CLightAttenuation& rAttenuation)
		{
			rAttenuation.constAttenuation = constantAttenuationNode.text().as_float(1.0f);
			rAttenuation.linearAttenuation = linearAttenuationNode.text().as_float(1.0f);
			rAttenuation.quadraticAttenuation = quadraticAttenuationNode.text().as_float(1.0f);
			
		}

		void COLLADAReader::parseOptics(pugi::xml_node& rOpticsNode, COptics& rOptics)
		{
			ZeroMemory(&rOptics, sizeof(COptics));
			pugi::xml_node perspectiveNode = rOpticsNode.child("technique_common").child("perspective");

			if(!perspectiveNode.empty())
			{
				parsePerspectiveProjection(perspectiveNode, rOptics.perspectiveProjection);
			}
		}

		void COLLADAReader::parsePerspectiveProjection(pugi::xml_node& rProjNode, CPerspective& rProj)
		{
			// default aspect ratio is 1.0f
			rProj.aspectRatio = rProjNode.child("aspect_ratio").text().as_float(1.0f);

			// default yfov is 45 graduses
			rProj.yfov = rProjNode.child("yfov").text().as_float(45.0f);

			rProj.zfar = rProjNode.child("zfar").text().as_float();
			rProj.znear = rProjNode.child("znear").text().as_float();
		}
	}
}
