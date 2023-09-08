#version 420 core

layout(triangle_strip, max_vertices = 3) out;
out GS_OUT
{
	vec2 texCoord;
	vec3 normal;

} gs_out;

layout(triangles) in;
in VS_OUT
{
	vec2 texCoord;

} gs_in[];

uniform float time;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}


vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 4.0;
	vec3 direction = normal * (sin(time / 2) + 1.0) * magnitude;
	direction.z /= 100;
	return position + vec4(direction, 0.0);
}


void main()
{
	vec3 normal = getNormal();
	gs_out.normal = normal;

	gl_Position = explode(gl_in[0].gl_Position, normal);
	gs_out.texCoord = gs_in[0].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	gs_out.texCoord = gs_in[1].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	gs_out.texCoord = gs_in[2].texCoord;
	EmitVertex();

	EndPrimitive();
}