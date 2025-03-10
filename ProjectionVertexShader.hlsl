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
    int clicked;
    float3 padding;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Position = mul(input.Position, MVP);
    //output.Position = input.Position;
    float4 highlight;
    if (clicked==1)
        highlight = float4(0.5f, 0.5f, 0, -0.3f);
    else
        highlight = float4(0, 0, 0, 0);
    output.Color = input.Color + highlight;
    return output;
}