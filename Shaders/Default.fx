cbuffer Global	
{
	float4x4 worldMatrix : World;
	float4x4 matWorldViewProjection : WorldViewProjection;	
	float3 lightDir : LightDirection;
	float4 lightColor : LightColor;
	float3 viewDir : ViewDirection;
}

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register ( s0 );

struct VS_INPUT 
{
   float3 Position : POSITION;
   float3 Normal : NORMAL;
   float2 tex : TEXCOORD;
};

struct VS_OUTPUT 
{
   float4 Position: SV_POSITION;
   linear float3 Normal : NORMAL;
   linear float3 lightDirection : NORMAL1;
   linear float distance : NORMAL2;
   float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul(float4(Input.Position, 1.0f), worldMatrix);

   Output.lightDirection = normalize(float4(0.0f, 5000.0f, 0.0f, 1.0f) - Output.Position);
   Output.distance = distance(Output.Position, float3(0.0f, 5000.0f, 0.0f));

   Output.Position = mul(Output.Position, matWorldViewProjection);
   Output.Normal = normalize(mul(Input.Normal, (float3x3)worldMatrix));
   
  
   Output.texCoord = Input.tex;

   return( Output );
   
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	linear float3 Normal : NORMAL;
	linear float3 lightDirection : NORMAL1;
	linear float distance : NORMAL2;
	float2 texCoord : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_TARGET
{  
	float3 lightDirection = normalize(input.lightDirection);
	float4 textureColor = txDiffuse.Sample(samLinear, input.texCoord);
	float3 reflectedRay = normalize(reflect(lightDirection, input.Normal));

	float cosineSpecular = dot(reflectedRay, viewDir) / (length(reflectedRay) * length(viewDir));
	float cosineDiffuse = dot(input.Normal, lightDirection) / (length(input.Normal) * length(lightDirection));

	cosineSpecular = saturate(cosineSpecular);
	cosineDiffuse  = saturate(cosineDiffuse);

	float4 resultColor = lightColor * cosineDiffuse + lightColor * pow(cosineSpecular, 50);

	saturate(resultColor);

	//resultColor /= (input.distance / 100);
	resultColor *= textureColor;
	
	return resultColor; 
}

technique11 Effect
{
   pass p0
   {
		SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetPixelShader( CompileShader( ps_5_0, PS() ) );
   }

}

