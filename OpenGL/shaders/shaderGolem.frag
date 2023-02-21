#version 410 core

in vec3 normal;
in vec4 fragPosEye;
in vec4 fragPosLightSpace;
in vec2 fragTexCoords;

out vec4 fColor;

//lighting
uniform	mat3 normalMatrixGolem;
uniform mat3 lightDirGolemMatrixGolem;
uniform	vec3 lightColorGolem;
uniform	vec3 lightDirGolem;
uniform sampler2D diffuseTextureGolem;
uniform sampler2D specularTextureGolem;
uniform sampler2D shadowMapGolem;

vec3 ambient;
float ambientStrength = 0.0f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 98.0f;

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrixGolem * normal);	
	
	//compute light direction
	vec3 lightDirGolemN = normalize(lightDirGolemMatrixGolem * lightDirGolem);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirGolemN + viewDirN);
		
	//compute ambient light
	ambient = ambientStrength * lightColorGolem;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirGolemN), 0.0f) * lightColorGolem;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColorGolem;
}

float computeShadow()
{	
	// perform perspective divide
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(normalizedCoords.z > 1.0f)
        return 0.0f;
    // Transform to [0,1] range
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMapGolem, normalizedCoords.xy).r;    
    // Get depth of current fragment from light's perspective
    float currentDepth = normalizedCoords.z;
    // Check whether current frag pos is in shadow
    float bias = 0.005f;
    float shadow = currentDepth - bias> closestDepth  ? 1.0f : 0.0f;

    return shadow;	
}



float computeFog()
{
 float fogDensity = 0.008f;
 float fragmentDistance = length(fragPosEye);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

 return clamp(fogFactor, 0.0f, 1.0f);
}


void main() 
{
	computeLightComponents();
	
	float shadow = computeShadow();
	

	//ambient *= vec3(texture(diffuseTextureGolem, fragTexCoords));
	//diffuse *= vec3(texture(diffuseTextureGolem, fragTexCoords));

	//specular *= vec3(texture(specularTextureGolem, fragTexCoords));

	ambient *= texture(diffuseTextureGolem, fragTexCoords).rgb;
	diffuse *= texture(diffuseTextureGolem, fragTexCoords).rgb;
	specular *= texture(specularTextureGolem, fragTexCoords).rgb;
	//modulate with shadow
	//vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    
        vec3 color = min((ambient + diffuse) + specular, 1.0f);        
	
	
	float fogFactor = computeFog();
	vec3 fogColor = vec3(0.6, 0.6, 0.8);

	//fColor = vec4(mix(fogColor, color, fogFactor), 1.0);
	//fColor = vec4(color, 1.0f);
	//fColor = vec4(fogColor * (1-fogFactor) + color * fogFactor ,1.0);
        fColor = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);


}
