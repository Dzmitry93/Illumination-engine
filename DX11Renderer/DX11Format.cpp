#include "DX11Format.h"

namespace OGE
{
	namespace render
	{
		ID3D11InputLayout* DX11Format::createInputLayout(ID3D11Device* pDevice, 
															BUFFER_FORMAT format, 
															void* pBytecode, 
															uint32 length)
		{
			ID3D11InputLayout* pInputLayout = NULL;

			switch(format)
			{
				case BUFFER_FORMAT::VERTEX_FORMAT_V3TNT2 :
					pDevice->CreateInputLayout(V3TNT2FormatInput, 5  , 
						pBytecode, length, &pInputLayout); break;
				case BUFFER_FORMAT::VERTEX_FORMAT_V3N3T2 :
					pDevice->CreateInputLayout(defaultFormatInput, 3  , 
						pBytecode, length, &pInputLayout); break;

				case BUFFER_FORMAT::VERTEX_FORMAT_V3T2 :
					pDevice->CreateInputLayout(V3T2FormatInput, 2	, 
						pBytecode, length, &pInputLayout); break;

				case BUFFER_FORMAT::VERTEX_FORMAT_V4T2 :
					pDevice->CreateInputLayout(V4T2FormatInput, 2	, 
						pBytecode, length, &pInputLayout); break;

				case BUFFER_FORMAT::VERTEX_FORMAT_V4 :
					pDevice->CreateInputLayout(V4FormatInput, 1	, 
						pBytecode, length, &pInputLayout); break;
			};

			return pInputLayout;
		}	

		DXGI_FORMAT DX11Format::toDXGIFormat(BUFFER_FORMAT format)
		{
			DXGI_FORMAT dxgiFormat;

			switch(format)
			{
				default : dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			};

			return dxgiFormat;
		}

		BUFFER_FORMAT DX11Format::toFormat(DXGI_FORMAT format)
		{
			BUFFER_FORMAT Format;
			switch(format)
			{
				default : 
					Format = BUFFER_FORMAT::FORMAT_UNKNOW; break;
			};

			return Format;
		}

		D3D11_PRIMITIVE_TOPOLOGY DX11Format::getDX11Topology(PRIMITIVE_TOPOLOGY topology)
		{
			D3D11_PRIMITIVE_TOPOLOGY d3dTopology;
			switch(topology)
			{
				case PRIMITIVE_TOPOLOGY::TRIANGLE_LIST : d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
				case PRIMITIVE_TOPOLOGY::TRIANGLE_STRIP : d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
				case PRIMITIVE_TOPOLOGY::LINE_LIST : d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
				case PRIMITIVE_TOPOLOGY::LINE_STRIP : d3dTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
			};

			return d3dTopology;
		}
	}
}