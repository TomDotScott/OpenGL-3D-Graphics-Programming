#version 440

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuse_tex;
	sampler2D specular_tex;
};

in vec3 varying_position;
in vec3 varying_colour;
in vec2 varying_texcoord;
in vec3 varying_normal;

out vec4 fragment_colour;

uniform Material material;

uniform vec3 light_position;
uniform vec3 camera_position;

vec3 calculate_ambient_colour(Material mat)
{
	return mat.ambient;
}

vec3 calculate_diffuse_colour(Material mat, vec3 position, vec3 normal, vec3 lightPos){
	// Diffuse lighting
	vec3 positionToLightDirectionVector = normalize(position - lightPos);
	 // The dot product gives use the the diffuse amount. The dot product goes between -1 and 1, we don't want negatives so we clamp
	float diffuseAmount = clamp(dot(positionToLightDirectionVector, normal), 0, 1);

	return mat.diffuse * diffuseAmount; 
}

vec3 calculate_specular_colour(Material mat, vec3 position, vec3 normal, vec3 lightPos, vec3 cameraPos){
	vec3 lightToPositionDirectionVector = normalize(lightPos - position);
	vec3 reflectionDirectionVector = normalize(reflect(lightToPositionDirectionVector, normalize(normal)));
	vec3 positionToViewDirectionVector = normalize(position - cameraPos);
	float specularConstant = pow(max(dot(positionToViewDirectionVector, reflectionDirectionVector), 0), 30);
	return material.specular * specularConstant;
}

void main()
{
	vec3 ambientFinal = calculate_ambient_colour(material); // Ambient light is the "natural" light of the scene
	vec3 diffuseFinal = calculate_diffuse_colour(material, varying_position, varying_normal, light_position);
	vec3 specularFinal = calculate_specular_colour(material, varying_position, varying_normal, light_position, camera_position);

//	fragment_colour = texture(material.diffuse_tex, varying_texcoord) * vec4(varying_colour, 1.f) * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	fragment_colour = texture(material.diffuse_tex, varying_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}