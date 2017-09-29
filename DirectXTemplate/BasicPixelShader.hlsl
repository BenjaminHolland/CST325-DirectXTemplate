cbuffer TimeBuffer:register(b0) {
	float Time;
}
static const float PI = 3.14159265f;
struct Input {
	float4 pos:SV_POSITION;
	float4 col:COLOR;
};
float4 main(Input v) : SV_TARGET
{
	float r = 0.5 + 0.1*sin(Time*0.01);
	float g = 0.5 + 0.1*sin(Time*0.01 + (PI * 2) / 10.0);
	float b = 0.5 + 0.3*sin(Time*0.01+(PI * 2) / 5.0);
	return float4(r*v.col.x, g*v.col.y, b*v.col.z, 1.0f);
}