#ifdef GEOMETRY_PASS
cbuffer Transform 
{
	matrix WorldMatrix;
	matrix WorldViewMatrix;
	matrix WorldViewProjMatrix;
};

cbuffer Material
{
	float3 SpecularAlbedo;
	float SpecularPower;
};

Texture2D DiffuseMap;
Texture2D NormalMap;	
SamplerState AnisoSampler;	
									 
struct VS_INPUT
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 binormal		: BINORMAL;
	float2 texcoord		: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 positionCS	: SV_POSITION;
	float4 positionWS	: POSITION;
	float4 normal		: NORMAL;
	float4 tangent		: TANGET;
	float4 binormal		: BINORMAL;
	float2 texcoord		: TEXCOORD;
};

struct PS_INPUT
{
	float4 positionCS	: SV_POSITION;
	float4 positionWS	: POSITION;
	float4 normal		: NORMAL;
	float4 tangent		: TANGET;
	float4 binormal		: BINORMAL;
	float2 texcoord		: TEXCOORD;
};

struct PS_OUTPUT
{
	float4 position			: SV_TARGET0;
	float4 normal			: SV_TARGET1;
	float4 diffuseAlbedo	: SV_TARGET2;
	float4 specularAlbedo	: SV_TARGET3;

};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionCS = mul(WorldViewProjMatrix, float4(input.position, 1.0f));
	output.positionWS = mul(WorldMatrix, float4(input.position, 1.0f));
	output.normal = float4(normalize(mul((float3x3)WorldMatrix, input.normal)), 1.0f);
	output.tangent = float4(normalize(mul((float3x3)WorldMatrix, input.tangent)), 1.0f);
	output.binormal = float4(normalize(mul((float3x3)WorldMatrix, input.binormal)), 1.0f);
	output.texcoord = input.texcoord;

	return output;
}

PS_OUTPUT ps_main(PS_INPUT input)
{
	PS_OUTPUT output;

	float3x3 tangentMatrix = float3x3(		normalize(input.tangent.xyz), 
											normalize(input.binormal.xyz),
											normalize(input.normal.xyz));				

	float3 normal = NormalMap.Sample(AnisoSampler, input.texcoord).rgb;

	output.diffuseAlbedo = float4(DiffuseMap.Sample(AnisoSampler, input.texcoord).rgb, 1.0f);
	output.specularAlbedo = float4(SpecularAlbedo, SpecularPower);
	output.position = input.positionWS;
	output.normal = float4(mul(normal, tangentMatrix), 1.0f);

	return output;
}

technique11 Effect
{
   pass p0
   {
		SetVertexShader ( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader ( NULL );
		SetPixelShader  ( CompileShader( ps_5_0, ps_main() ) );
   }
}

#endif

#ifdef LIGHT_PASS

Texture2D PositionMap;
Texture2D NormalMap;
Texture2D DiffuseMap;
Texture2D SpecularMap;
TextureCube ShadowMap;

cbuffer LightParams
{
	float4 LightDiffuseColor;
	float4 LightSpecularColor;
	float4 Spotparams;
	float4 LightAttenuation;
	float4 LightPosition; 
	float4 LightDirection;

	matrix ShadowProjMatrix;
};		  

cbuffer Camera
{
	float3 CameraDir;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.texcoord = input.texcoord;

	return output;
}

SamplerState AnisoSampler;
SamplerState g_samCube
{
    Filter = ANISOTROPIC;
    AddressU = Clamp;
    AddressV = Clamp;
};


float4 ps_main(PS_INPUT input) : SV_TARGET
{
	float3 normal = NormalMap.Sample(AnisoSampler, input.texcoord).xyz;
	float3 position = PositionMap.Sample(AnisoSampler, input.texcoord).xyz;
	float3 diffuseAlbedo = DiffuseMap.Sample(AnisoSampler, input.texcoord).xyz;
	float3 specularAlbedo = SpecularMap.Sample(AnisoSampler, input.texcoord).xyz;
	float specularPower = SpecularMap.Sample(AnisoSampler, input.texcoord).w;

	float3 lightDirection = 0.0f;
	float attenuation = 1.0f;

	#if POINT_LIGHT	|| SPOTLIGHT
		lightDirection = (LightPosition - float4(position, 1.0f)).xyz;
		float dst = length(lightDirection);

		float depth = ShadowMap.Sample(AnisoSampler, -lightDirection);

		attenuation = 1.0f / 
			(LightAttenuation.x + dst * (LightAttenuation.y + dst * LightAttenuation.z));
		lightDirection /= dst;
		
	#elif DIRECTIONAL_LIGHT
		lightDirection = -LightDirection;
	#endif

	#ifdef SPOTLIGHT
		float spotFactor = saturate(dot(LightDirection, -lightDirection));
		attenuation *= pow(spotFactor, Spotparams.y);
	#endif

	float cosineSpecular = 0.0f;
	float cosineDiffuse = saturate(dot(normal, lightDirection));

	if(dst - depth > 0.1f)
	{
		attenuation = 0.1f;
	}

	if( dot( lightDirection, normal ) > 0)
	{
		float3 reflectedRay = normalize( reflect( lightDirection, normal ) ) ;
		cosineSpecular = saturate(dot(reflectedRay, CameraDir));
	}

	return saturate(float4(
		(cosineDiffuse * LightDiffuseColor * diffuseAlbedo +
		specularAlbedo * pow(cosineSpecular, specularPower)	* LightSpecularColor)
		* attenuation, 1.0f));
}

technique11 Effect
{
   pass p0
   {
		SetVertexShader ( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader ( NULL );
		SetPixelShader  ( CompileShader( ps_5_0, ps_main() ) );
   }
}

#endif

#ifdef SHADOW_MAP_PASS

cbuffer Transform 
{
	matrix WorldMatrix;
	matrix WorldViewMatrix;
	matrix WorldViewProjMatrix;
};

struct VS_INPUT
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 binormal		: BINORMAL;
	float2 texcoord		: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position		: SV_POSITION;
	float4 positionW	: POSITION;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(WorldMatrix, float4(input.position, 1.0f));
	output.positionW = output.position;

	return output;
} 

#if POINT_LIGHT

cbuffer ShadowData
{
	matrix projection;
	matrix viewArray[6];
	float4 lightPos;
}

struct PS_INPUT
{
	float4 position		: SV_POSITION;
	float4 positionW	: POSITION;
	uint RTIndex		: SV_RENDERTARGETARRAYINDEX;
};

[maxvertexcount(18)]
void gs_main( 
			triangle VS_OUTPUT input[3],
			inout TriangleStream<PS_INPUT> cubeMapStream)
{
	for(uint rtIndex = 0; rtIndex < 6; rtIndex++)
	{
		PS_INPUT output;

		for(uint i = 0; i < 3; i++)
		{
			output.positionW = input[i].positionW;
			output.position = mul(viewArray[rtIndex], input[i].position);
			output.position = mul(projection, output.position);
			output.RTIndex = rtIndex;

			cubeMapStream.Append(output);
		}

		cubeMapStream.RestartStrip();
	}

}

float ps_main(PS_INPUT input) : SV_TARGET
{
	return length(input.positionW - lightPos);
}

technique11 Effect
{
   pass p0
   {
		SetVertexShader ( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader ( CompileShader( gs_5_0, gs_main() ) );
		SetPixelShader  ( CompileShader( ps_5_0, ps_main() ) );
   }
}

#else

technique11 Effect
{
   pass p0
   {
		SetVertexShader ( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader ( NULL );
		SetPixelShader  ( CompileShader( ps_5_0, ps_main() ) );
   }
}
#endif

#endif

#ifdef BOUNDING_BODY

struct VS_INPUT 
{
	float4 position : POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

cbuffer Transform 
{
	matrix WorldMatrix;
	matrix WorldViewMatrix;
	matrix WorldViewProjMatrix;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	input.position = mul(WorldViewProjMatrix, input.position);
	
	return input;
}

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}

technique11 Effect
{
   pass p0
   {
		SetVertexShader ( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader ( NULL );
		SetPixelShader  ( CompileShader( ps_5_0, ps_main() ) );
   }
}

#endif