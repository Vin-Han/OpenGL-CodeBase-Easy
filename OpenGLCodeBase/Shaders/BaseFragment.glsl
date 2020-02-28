# version 150 core

uniform sampler2D		diffuseTex;
uniform sampler2DShadow shadowTex;

uniform vec4	 objectColor;

uniform vec4	 lightColour;
uniform vec3	 lightPos;
uniform float	 lightRadius;

uniform vec3	 camPos;

in Vertex{
vec3 normal;
vec2 texCoord;
vec4 shadowProj;
vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {

	float shadow = 1.0;
	if (IN.shadowProj.w > 0) {
		//shadow = textureProj(shadowTex, IN.shadowProj);
	}


	vec3  incident = normalize(lightPos - IN.worldPos);
	float lambert = max(0.0, dot(incident, IN.normal)) * 0.9;

	lambert *= shadow;
	
	vec3 viewDir = normalize(camPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);


	float rFactor = max(0.0, dot(halfDir, IN.normal));
	float sFactor = pow(rFactor, 80.0);
	

	vec4 texColour = texture(diffuseTex, IN.texCoord);

	fragColour = vec4(texColour.rgb ,1.0);
	//fragColour += vec4(texColour.rgb* lightColour.rgb * lambert,1.0);
}
