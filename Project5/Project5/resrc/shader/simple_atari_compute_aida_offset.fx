sampler sdayo : register(s0);

sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};


struct AtariUnitInfo {
  int atari_idx;
  int vertexs_place;
  int indexs_place;
  int vertex_count;
  int index_count;
  column_major float4x4 world;
  float3 jyusin;
  float3 v;
  float r;
  float dt;
};

struct AtariUnitKumi {
  int atari_idx;
  int atari_idx2;
};

struct AtariUnitAns {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int obb_idx2;
  float3 kouten_jyusin;
  float3 kouten_housen;
  int is_use;
};

struct AtariUnitOBB {
  float3 c;
  float3 u[3];
  float3 e;
};

struct AtariUnitTikeiToSoreigai {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int offset;
};

struct AtariUnitTikeiIgaiDousi {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int obb_idx2;
};

cbuffer CB0 : register(b0) {
  int vertexs_count;
  int indexs_count;
  int auinfo_count;
  int obbs_count;
  int kumi_count;
  int ans_count;
  int igaidousi_count;
  int soreigai_count;
};

cbuffer CB1 : register(b1) {
  int offset_atari;
  int atari_offset; // KTROBO_ATARI_OFFSET 512
  int nasi;
  int nasi2;
};

struct MESH_VERTEXKARI {
  float4 pos;
  float4 normal;
};

struct MESH_INDEX {
  uint index;
};


StructuredBuffer<MESH_VERTEXKARI> vertexBuf : register(t0);
StructuredBuffer<MESH_INDEX> indexBuf : register(t1);
StructuredBuffer<AtariUnitOBB> obbBuf : register(t2);
StructuredBuffer<AtariUnitInfo> auinfoBuf: register(t3);
StructuredBuffer<AtariUnitKumi> kumiBuf : register(t4);
StructuredBuffer<AtariUnitTikeiIgaiDousi> autidBuf : register(t6);
StructuredBuffer<AtariUnitTikeiToSoreigai> auttsBuf : register(t5);


RWStructuredBuffer<AtariUnitAns> ansBuf :register(u0);
RWStructuredBuffer<AtariUnitAns> ansBuf2 : register(u1);
RWStructuredBuffer<AtariUnitAns> ansBuf3: register(u2);



// group 縺ｫ縺ｯmax繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ謨ｰ繧呈欠螳壹☆繧・
// numthreads 縺ｫ縺ｯmaxbonenum 縺ｮ謨ｰ繧呈欠螳壹☆繧・
[numthreads(16,8,1)]
void CalcCS(uint3 gtid: SV_GroupID, uint gid: SV_GroupIndex )
{

uint struct_idx = ansBuf3.IncrementCounter();//(gtid.x+ gtid.y * 32 + gtid.z *32*32)*128+ gid;

if (ans_count > struct_idx) {

  if (struct_idx >= 0) {//offset_atari) {
    if (struct_idx < atari_offset) {// offset_atari+atari_offset) {
	 
	   uint idxd = struct_idx;
       uint str_idx = struct_idx + offset_atari;  
       if (ansBuf2[str_idx].is_use) {
		  ansBuf3[idxd].is_use = ansBuf2[str_idx].is_use;
          ansBuf3[idxd].atari_idx = ansBuf2[str_idx].atari_idx;
          ansBuf3[idxd].atari_idx2 =ansBuf2[str_idx].atari_idx2;
          ansBuf3[idxd].obb_idx = ansBuf2[str_idx].obb_idx;
          ansBuf3[idxd].obb_idx2 = ansBuf2[str_idx].obb_idx2;
          ansBuf3[idxd].kouten_jyusin = ansBuf2[str_idx].kouten_jyusin;
          ansBuf3[idxd].kouten_housen = ansBuf2[str_idx].kouten_housen;
       } else {
          ansBuf3[idxd].is_use = 0;
       }
    }
  }
}

}












