cbuffer TimeBuffer:register(b0) {
	float Time;
}
struct Input {
	float4 pos:POSITION;
	float4 col:COLOR;
};
struct Output {
	float4 pos:SV_POSITION;
	float4 col:COLOR;
};
Output main( Input v ) 
{
	return v;
}