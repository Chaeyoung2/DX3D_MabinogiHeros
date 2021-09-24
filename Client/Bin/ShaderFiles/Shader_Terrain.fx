matrix			g_matWorld, g_matView, g_matProj;

vector			g_vCameraPos;

vector			g_vLightDir;
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlAmbient = vector(0.2f, 0.2f, 0.2f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

float			g_fPower = 30.f;


texture			g_BaseTexture;
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_MixTexture_1;
sampler MixSampler_1 = sampler_state
{
	texture = g_MixTexture_1;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_MixTexture_3;
sampler MixSampler_3 = sampler_state
{
	texture = g_MixTexture_3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_FilterTexture;
sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
	vector		vSpecular : TEXCOORD1;
	vector		vWorldPos : TEXCOORD2;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0.f;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f
		, normalize(vWorldNormal)), 0.0f);

	vector		vReflect = reflect(g_vLightDir, vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCameraPos;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = vWorldPos;

	return Out;
}

struct PS_IN
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR;
	float2		vTexUV : TEXCOORD0;
	vector		vSpecular : TEXCOORD1;
	vector		vWorldPos : TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;



	vector		vColor[4];

	vColor[0] = tex2D(BaseSampler, In.vTexUV * 20.f);
	vColor[1] = tex2D(MixSampler_1, In.vTexUV * 20.f);
	vColor[2] = tex2D(MixSampler_3, In.vTexUV * 20.f);
	vColor[3] = tex2D(FilterSampler, In.vTexUV);

	//vColor[3] = (vector)0.f;	// 브러쉬 안 쓸거얌.

	//if (g_vBrushPos.x - g_fRadius * 0.5f < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fRadius * 0.5f
	//	&& g_vBrushPos.z - g_fRadius * 0.5f < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fRadius * 0.5f)
	//{
	//	float2		vTexUV;
	//	vTexUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRadius * 0.5f)) / g_fRadius;
	//	vTexUV.y = ((g_vBrushPos.z + g_fRadius * 0.5f) - In.vWorldPos.z) / g_fRadius;
	//	vColor[3] = tex2D(BrushSampler, vTexUV);
	//}

	vector 	vResultColor = vColor[0] * vColor[3].r + vColor[1] * (1.f - vColor[3].r) /*+ vColor[3]*/;


	Out.vColor = vResultColor * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade
		+ (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular;

	return Out;
}

technique DefaultDevice
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}
