# version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat4 shadowMatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out Vertex{
vec3 normal;
vec2 texCoord;
vec4 shadowProj;
vec3 worldPos;
} OUT;

void main(void) {

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));

	OUT.texCoord = texCoord;

	//OUT.shadowProj = shadowMatrix * vec4(position, 1);
	OUT.shadowProj = (shadowMatrix * vec4(position + (normal * 1.5), 1));

	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	mat4 mvp = (projMatrix * viewMatrix * modelMatrix);
	gl_Position = mvp * vec4(position, 1.0);
}