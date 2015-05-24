$input v_normal

#include "bgfx/common.sh"

uniform vec3 u_lightDir;

void main()
{
	vec3 n = normalize(v_normal);
	vec3 l = normalize(u_lightDir);
	float cosTheta = dot(n, l);
	gl_FragColor = vec4(vec3_splat(cosTheta), 1.0);
}
