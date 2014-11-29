
cbuffer PerFrameBuffer
{
	matrix screenMatrix : ScreenMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

PixelInputType FontVertexShader(VertexInputType input)
{
    PixelInputType output;
    input.position.w = 1.0f;
    output.position = mul(input.position, screenMatrix);
	output.tex = input.tex;
    
    return output;
}

Texture2D fontTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
    float4 pixelColor;
};

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	
	
	// Sample the texture pixel at this location.
	color = fontTexture.Sample(SampleType, input.tex);
	
	// If the color is black on the texture then treat this pixel as transparent.
	if(color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	
	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	else
	{
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

    return color;
}

technique11
{
	pass p0
	{
		SetVertexShader( CompileShader ( vs_5_0, FontVertexShader() ) );
		SetPixelShader( CompileShader ( ps_5_0, FontPixelShader() ) );
	}
}