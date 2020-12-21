#version 440

in vec3 varying_position;
in vec3 varying_colour;
in vec2 varying_texcoord;
in vec3 varying_normal;

out vec4 fragment_colour;

// Samples one pixel at a time from the image data
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 light_position;
uniform vec3 camera_position;

void main()
{
	// fragment_colour = vec4(varying_colour, 1.f);
	// fragment_colour = texture(texture0, varying_texcoord) * texture(texture1, varying_texcoord);

	// Ambient lighting 
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f); // Ambient light is the "natural" light of the scene

	// Diffuse lighting
	vec3 positionToLightDirectionVector = normalize(varying_position - light_position);

	vec3 diffuseColour = vec3(1.f, 1.f, 1.f); // No special colour for the diffuse light

	 // The dot product gives use the the diffuse amount. The dot product goes between -1 and 1, we don't want negatives so we clamp
	float diffuseAmount = clamp(dot(positionToLightDirectionVector, varying_normal), 0, 1);

	vec3 diffuseFinal = diffuseColour * diffuseAmount; 

	// Specular lighting
	vec3 lightToPositionDirectionVector = normalize(light_position - varying_position);
	vec3 reflectionDirectionVector = normalize(reflect(lightToPositionDirectionVector, normalize(varying_normal)));
	vec3 positionToViewDirectionVector = normalize(varying_position - camera_position);
	float specularConstant = pow(max(dot(positionToViewDirectionVector, reflectionDirectionVector), 0), 30);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;

	fragment_colour = texture(texture0, varying_texcoord) * vec4(varying_colour, 1.f) * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}