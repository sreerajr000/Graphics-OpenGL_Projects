#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D shadowMap;
uniform Light light;
//uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
vec3 lightPos = light.position;
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    
    //vec3 normal = normalize(fs_in.Normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    
    vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 lightDir = normalize(lightPos - fs_in.TangentFragPos);
    
    
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 0.75 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{           
    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;
    
    //vec3 normal = normalize(fs_in.Normal);
    
    vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.TexCoords).rgb;
    // diffuse
    
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fs_in.TexCoords).rgb;
    // specular
    
    //vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * texture(material.specular, fs_in.TexCoords).rgb;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);   
    //shadow = 0.0;         
    vec3 lighting = (ambient + vec3(1.0 - shadow) * (diffuse + specular));    
    
    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(diffuse, 1.0);
}