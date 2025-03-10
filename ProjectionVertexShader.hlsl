struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix MVP;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Position = mul(input.Position, MVP);
    //output.Position = input.Position;
    output.Color = input.Color;
    return output;
}