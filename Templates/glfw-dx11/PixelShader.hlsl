// PixelShader.hlsl

struct PSInput
{
    float4 position : SV_POSITION;
};

float4 main(PSInput input) : SV_TARGET
{
    // Set the pixel color to red (1, 0, 0, 1)
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
