#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// INST_POS                 0   xy          0     NONE   float   xy  
// DIR                      0   xy          1     NONE   float   xy  
// THICKNESS                0   x           2     NONE   float   x   
// COLOR                    0   xyzw        3     NONE   float   xyzw
// SV_VertexID              0   x           4   VERTID    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// POS                      0   xy          1     NONE   float   xy  
// IP                       0     zw        1     NONE   float     zw
// DIR                      0   xy          2     NONE   float   xy  
// THICKNESS                0     z         2     NONE   float     z 
// COLOR                    0   xyzw        3     NONE   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[4], immediateIndexed
dcl_input v0.xy
dcl_input v1.xy
dcl_input v2.x
dcl_input v3.xyzw
dcl_input_sgv v4.x, vertex_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output o1.zw
dcl_output o2.xy
dcl_output o2.z
dcl_output o3.xyzw
dcl_temps 2
dcl_indexableTemp x0[6], 4
mov x0[0].xy, l(-1.000000,-1.000000,0,0)
mov x0[1].xy, l(-1.000000,1.000000,0,0)
mov x0[2].xy, l(1.000000,1.000000,0,0)
mov x0[3].xy, l(-1.000000,-1.000000,0,0)
mov x0[4].xy, l(1.000000,1.000000,0,0)
mov x0[5].xy, l(1.000000,-1.000000,0,0)
mov r0.x, v4.x
mov r0.xy, x0[r0.x + 0].xyxx
dp2 r0.z, v1.xyxx, v1.xyxx
sqrt r0.z, r0.z
mul r1.xy, v1.yxyy, l(-1.000000, 1.000000, 0.000000, 0.000000)
div r0.zw, r1.xxxy, r0.zzzz
mul r0.xz, r0.zzwz, r0.xxxx
mul r0.xz, r0.xxzx, v2.xxxx
mad r0.xy, r0.yyyy, v1.xyxx, r0.xzxx
add r0.xy, r0.xyxx, v0.xyxx
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, cb0[0].xyzw, r0.xxxx, r1.xyzw
mov o1.xy, r0.xyxx
add o0.xyzw, r1.xyzw, cb0[3].xyzw
mov o1.zw, v0.xxxy
mov o2.xy, v1.xyxx
mov o2.z, v2.x
mov o3.xyzw, v3.xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE d3d11_line_vshader[] =
{
     68,  88,  66,  67, 163,  63, 
    159,  66, 191,  65,  55,  30, 
      1,  72,  37,  86, 230, 246, 
    241, 100,   1,   0,   0,   0, 
     52,   5,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
    224,   0,   0,   0, 168,   1, 
      0,   0,  73,  83,  71,  78, 
    172,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   3,   3,   0,   0, 
    137,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
    141,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   1,   1,   0,   0, 
    151,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,  15,   0,   0, 
    157,   0,   0,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   1,   1,   0,   0, 
     73,  78,  83,  84,  95,  80, 
     79,  83,   0,  68,  73,  82, 
      0,  84,  72,  73,  67,  75, 
     78,  69,  83,  83,   0,  67, 
     79,  76,  79,  82,   0,  83, 
     86,  95,  86, 101, 114, 116, 
    101, 120,  73,  68,   0, 171, 
    171, 171,  79,  83,  71,  78, 
    192,   0,   0,   0,   6,   0, 
      0,   0,   8,   0,   0,   0, 
    152,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
    164,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,  12,   0,   0, 
    168,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  12,   3,   0,   0, 
    171,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,  12,   0,   0, 
    175,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   4,  11,   0,   0, 
    185,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     80,  79,  83,   0,  73,  80, 
      0,  68,  73,  82,   0,  84, 
     72,  73,  67,  75,  78,  69, 
     83,  83,   0,  67,  79,  76, 
     79,  82,   0, 171,  83,  72, 
     69,  88, 132,   3,   0,   0, 
     80,   0,   1,   0, 225,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3,  18,  16,  16,   0, 
      2,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      3,   0,   0,   0,  96,   0, 
      0,   4,  18,  16,  16,   0, 
      4,   0,   0,   0,   6,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 194,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3,  66,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   3,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0, 105,   0,   0,   4, 
      0,   0,   0,   0,   6,   0, 
      0,   0,   4,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128, 191, 
      0,   0, 128, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128, 191, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128, 191, 
      0,   0, 128, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   9,  50,  48, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   4,   0, 
      0,   0,  54,   0,   0,   7, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,  48,  32,   4, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     15,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  75,   0, 
      0,   5,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     22,  21,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128, 191,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  14,   0, 
      0,   7, 194,   0,  16,   0, 
      0,   0,   0,   0,   6,   4, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     82,   0,  16,   0,   0,   0, 
      0,   0, 166,  11,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  82,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   2,  16,   0,   0,   0, 
      0,   0,   6,  16,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9,  50,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0, 134,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   8, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     54,   0,   0,   5, 194,  32, 
     16,   0,   1,   0,   0,   0, 
      6,  20,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  66,  32,  16,   0, 
      2,   0,   0,   0,  10,  16, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   3,   0,   0,   0, 
     70,  30,  16,   0,   3,   0, 
      0,   0,  62,   0,   0,   1
};
