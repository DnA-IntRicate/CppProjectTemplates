// PixelShader.hlsl

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct PSOutput
{
    float4 Color : COLOR;
};

PSOutput main(PSInput input) : SV_TARGET
{
    PSOutput output;
    output.Color = input.Color;

    return output;
}
