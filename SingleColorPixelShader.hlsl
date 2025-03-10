struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

cbuffer ConstantBuffer2 : register(b1)
{
    float4 Color;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    return Color;
}