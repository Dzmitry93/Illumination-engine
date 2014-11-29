#ifndef __DX11FORMAT_H__
#define __DX11FORMAT_H__

#include "Format.h"
#include <d3d11.h>

using OGE::resource::BUFFER_FORMAT;
using OGE::resource::PRIMITIVE_TOPOLOGY;
using OGE::resource::Format;

namespace OGE
{
	namespace render
	{
			static D3D11_INPUT_ELEMENT_DESC defaultFormatInput[] =  
			{
				{
					"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				}
			};

			static D3D11_INPUT_ELEMENT_DESC V3TNT2FormatInput[] = 
			{
				{
					"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				}
			};

			static  D3D11_INPUT_ELEMENT_DESC V3T2FormatInput[] = 
			{
				{
					"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				}
			};

			static  D3D11_INPUT_ELEMENT_DESC V4T2FormatInput[] = 
			{
				{
					"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
				},
				{
					"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
				}
			};

			static D3D11_INPUT_ELEMENT_DESC V4FormatInput[] =
			{
				{
					"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
				}
			};

		class DX11Format
		{
		public:

			static ID3D11InputLayout* createInputLayout(ID3D11Device* pDevice, 
															BUFFER_FORMAT format, 
															void* pBytecode, 
															uint32 length);

			static DXGI_FORMAT toDXGIFormat(BUFFER_FORMAT format);
			static BUFFER_FORMAT toFormat(DXGI_FORMAT format);
			static D3D11_PRIMITIVE_TOPOLOGY getDX11Topology(PRIMITIVE_TOPOLOGY topology);
		};
	}
}

#endif