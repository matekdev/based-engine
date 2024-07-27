cbuffer CameraMatrix : register(b0)
{
    matrix CameraViewMatrix;
    matrix CameraProjectionMatrix;
    float3 CameraPosition;
};
