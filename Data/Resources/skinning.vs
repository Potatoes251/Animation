
/////////////////////
// INPUT VARIABLES //
/////////////////////
in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
smooth out vec2 texCoord;
smooth out vec3 outNormal;

uniform SceneMatrices
{
	uniform mat4 projectionMatrix;
} sm;

uniform mat4 modelViewMatrix;

uniform SkinningMatrices
{
	uniform mat4 mat[64];
} skin;



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec4 skinnedPos = vec4(0.0);
    vec3 skinnedNormal = vec3(0.0);

	for (int i = 0; i < 4; i++)
    {
        int boneIndex = int(boneIndices[i]);
        float weight = boneWeights[i];

        mat4 boneMatrix = skin.mat[boneIndex];

        skinnedPos += boneMatrix * vec4(inputPosition, 1.0) * weight;
        skinnedNormal += mat3(boneMatrix) * normal * weight;
    }

	// Transform by model-view and projection
    gl_Position = sm.projectionMatrix * (modelViewMatrix * skinnedPos);
    outNormal = normalize(mat3(modelViewMatrix) * skinnedNormal);
}
