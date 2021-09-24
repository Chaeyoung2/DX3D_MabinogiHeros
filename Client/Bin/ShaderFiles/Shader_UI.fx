
// �������� : Ŭ���̾�Ʈ�κ��� ���� ���� �� �ִ�, ���
// �������� �������� ��Ƽ� ������̺�(������Ʈ���̺�)
matrix			g_matWorld, g_matView, g_matProj;
float				g_fData = 10.f;
float				g_fAlphaValue = 0.f;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

// ��ġ : Z������ �ϱ� �����ܰ�� ��ȯ�����ش�.
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV;
	matrix			matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector		vColor : COLOR;
};

// �ȼ� ���̴��� �����ϱ����� �Լ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0.f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.a -= g_fAlphaValue;

	/*Out.vColor.a = 1.f - In.vTexUV.y;*/


	//// �ȼ��� ���� �����Ѵ�.
	//if(In.vTexUV.y > 0.5f)
	//	Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
	//else
	//	Out.vColor = vector(1.f, 0.0f, 0.0f, 1.f);\]
	return Out;
}

technique Default_Device // ��ġ�� ���̴� ���� �������ο����� ����.
{
	pass Default_Rendering
	{
		cullmode = none;
		lighting = false;
		AlphaBlendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}




