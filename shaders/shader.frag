#version 450

layout(set=1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragViewDir;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

vec3 Oren_Nayar_Diffuse_BRDF(vec3 L, vec3 N, vec3 V, vec3 C, float sigma) {
	// Directional light direction
	// additional parameter:
	// float sigma : roughness of the material
	float theta_i = acos(dot(L, N));
	float theta_r = acos(dot(V, N));
	float alpha = max(theta_i, theta_r);
	float beta = min(theta_i, theta_r);
	float A = 1 - 0.5*(pow(sigma, 2)/(pow(sigma, 2)+0.33));
	float B = 0.45*(pow(sigma, 2)/(pow(sigma, 2)+0.09));
	vec3 v_i = normalize(L - dot(L, N)*N);
	vec3 v_r = normalize(V - dot(V, N)*N);
	float G = max(0, dot(v_i, v_r));
	vec3 L1 = C*clamp(dot(L, N), 0, 1);
	return L1*(A+B*G*sin(alpha)*tan(beta));
}

void main() {
	const vec3  diffColor = texture(texSampler, fragTexCoord).rgb;
	const vec3  specColor = vec3(1.0f, 1.0f, 1.0f);
	const float specPower = 150.0f;
	const vec3  L = vec3(-0.4830f, 0.8365f, -0.2588f);
	
	vec3 N = normalize(fragNorm);
	vec3 R = -reflect(L, N);
	vec3 V = normalize(fragViewDir);
	
	// Lambert diffuse
	//vec3 diffuse  = diffColor * max(dot(N,L), 0.0f) /*INCREASE POWER OF THE LIGHT*/ * 1.5f;
	vec3 diffuse = Oren_Nayar_Diffuse_BRDF(L, N, V, diffColor, 1.5);
	//Ambient light
	vec3 ambient  = vec3(0.3f, 0.3f, 0.1f) * diffColor;
	
	outColor = vec4(clamp(ambient + diffuse, vec3(0.0f), vec3(1.0f)), 1.0f);
}