cbuffer DirectionalLight : register(b0)
{
    float3 DirectionalLightDirection;
    float DirectionalLightPadding1;

    float3 DirectionalLightAmbient;
    float DirectionalLightPadding2;

    float3 DirectionalLightDiffuse;
    float DirectionalLightPadding3;

    float3 DirectionalLightSpecular;
    float DirectionalLightPadding4;
};
