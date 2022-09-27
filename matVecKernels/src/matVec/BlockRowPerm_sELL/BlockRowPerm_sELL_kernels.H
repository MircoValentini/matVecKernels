#ifndef __MATVEC_BlockRowPerm_sELL_KERNELS_H__
#define __MATVEC_BlockRowPerm_sELL_KERNELS_H__


#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
typedef void (*fcn_k)
(
        idx_k*         const                      cnt,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        idx_k          const * const __restrict__ neigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
);


template<const int NNZ>
inline void
product_sELL_AVX512
(
        idx_k*         const                      cnt,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        idx_k          const * const __restrict__ neigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{

  // printf( "AVX512 kernel: %10lu, %10lu, %10lu\n", NNZ, rlo, rhi );
  idx_k iRow;
  idx_k iCol;

  idx_k iNei0;
  idx_k iNei1;
  idx_k iNei2;
  idx_k iNei3;
  idx_k iNei4;
  idx_k iNei5;
  idx_k iNei6;
  idx_k iNei7;

  fld_k xr[8];
  fld_k tmp_psi[8];

  fld_k tmp_upper[8];
  fld_k tmp_lower[8];
  fld_k upperProduct[8];
  fld_k lowerProduct[8];

  for ( iRow=rlo; iRow<rhi; iRow=iRow+8 ) // Loop over the rows
  {
    // row indices for the AVX rows
    const idx_k rid0 = iRow+0;
    const idx_k rid1 = iRow+1;
    const idx_k rid2 = iRow+2;
    const idx_k rid3 = iRow+3;
    const idx_k rid4 = iRow+4;
    const idx_k rid5 = iRow+5;
    const idx_k rid6 = iRow+6;
    const idx_k rid7 = iRow+7;
    // Packing right data
    xr[0] = psi[rid0];
    xr[1] = psi[rid1];
    xr[2] = psi[rid2];
    xr[3] = psi[rid3];
    xr[4] = psi[rid4];
    xr[5] = psi[rid5];
    xr[6] = psi[rid6];
    xr[7] = psi[rid7];
    // Initialize psi AVX buffer
    tmp_psi[0] = 0.0;
    tmp_psi[1] = 0.0;
    tmp_psi[2] = 0.0;
    tmp_psi[3] = 0.0;
    tmp_psi[4] = 0.0;
    tmp_psi[5] = 0.0;
    tmp_psi[6] = 0.0;
    tmp_psi[7] = 0.0;
    // Initialize tmp_upper AVX buffer
    tmp_upper[0] = 0.0;
    tmp_upper[1] = 0.0;
    tmp_upper[2] = 0.0;
    tmp_upper[3] = 0.0;
    tmp_upper[4] = 0.0;
    tmp_upper[5] = 0.0;
    tmp_upper[6] = 0.0;
    tmp_upper[7] = 0.0;
    // Initialize tmp_lower AVX buffer
    tmp_lower[0] = 0.0;
    tmp_lower[1] = 0.0;
    tmp_lower[2] = 0.0;
    tmp_lower[3] = 0.0;
    tmp_lower[4] = 0.0;
    tmp_lower[5] = 0.0;
    tmp_lower[6] = 0.0;
    tmp_lower[7] = 0.0;
    // Initialize upperProduct AVX buffer
    upperProduct[0] = 0.0;
    upperProduct[1] = 0.0;
    upperProduct[2] = 0.0;
    upperProduct[3] = 0.0;
    upperProduct[4] = 0.0;
    upperProduct[5] = 0.0;
    upperProduct[6] = 0.0;
    upperProduct[7] = 0.0;
    // Initialize neighbour indices AVX buffer
    // iNei0 = clo;
    // iNei1 = clo;
    // iNei2 = clo;
    // iNei3 = clo;
    // iNei4 = clo;
    // iNei5 = clo;
    // iNei6 = clo;
    // iNei7 = clo;
    // Loop over the non-zeroes elements in the lines
    for ( iCol=0; iCol<NNZ; ++iCol)
    {
      // Constructing indices
      const idx_k cid0 = *cnt+0;
      const idx_k cid1 = *cnt+1;
      const idx_k cid2 = *cnt+2;
      const idx_k cid3 = *cnt+3;
      const idx_k cid4 = *cnt+4;
      const idx_k cid5 = *cnt+5;
      const idx_k cid6 = *cnt+6;
      const idx_k cid7 = *cnt+7;
      // Handling indices
      iNei0 = neigh[cid0];
      iNei1 = neigh[cid1];
      iNei2 = neigh[cid2];
      iNei3 = neigh[cid3];
      iNei4 = neigh[cid4];
      iNei5 = neigh[cid5];
      iNei6 = neigh[cid6];
      iNei7 = neigh[cid7];
      // packing psi data
      tmp_psi[0] = psi[iNei0];
      tmp_psi[1] = psi[iNei1];
      tmp_psi[2] = psi[iNei2];
      tmp_psi[3] = psi[iNei3];
      tmp_psi[4] = psi[iNei4];
      tmp_psi[5] = psi[iNei5];
      tmp_psi[6] = psi[iNei6];
      tmp_psi[7] = psi[iNei7];
      // packing upper data
      tmp_upper[0] = upper[cid0];
      tmp_upper[1] = upper[cid1];
      tmp_upper[2] = upper[cid2];
      tmp_upper[3] = upper[cid3];
      tmp_upper[4] = upper[cid4];
      tmp_upper[5] = upper[cid5];
      tmp_upper[6] = upper[cid6];
      tmp_upper[7] = upper[cid7];
      // packing lower data
      tmp_lower[0] = lower[cid0];
      tmp_lower[1] = lower[cid1];
      tmp_lower[2] = lower[cid2];
      tmp_lower[3] = lower[cid3];
      tmp_lower[4] = lower[cid4];
      tmp_lower[5] = lower[cid5];
      tmp_lower[6] = lower[cid6];
      tmp_lower[7] = lower[cid7];
      // Compute upper FMS operation
      upperProduct[0] += tmp_upper[0]*tmp_psi[0];
      upperProduct[1] += tmp_upper[1]*tmp_psi[1];
      upperProduct[2] += tmp_upper[2]*tmp_psi[2];
      upperProduct[3] += tmp_upper[3]*tmp_psi[3];
      upperProduct[4] += tmp_upper[4]*tmp_psi[4];
      upperProduct[5] += tmp_upper[5]*tmp_psi[5];
      upperProduct[6] += tmp_upper[6]*tmp_psi[6];
      upperProduct[7] += tmp_upper[7]*tmp_psi[7];
      // Compute the lower product
      lowerProduct[0] = tmp_lower[0]*xr[0];
      lowerProduct[1] = tmp_lower[1]*xr[1];
      lowerProduct[2] = tmp_lower[2]*xr[2];
      lowerProduct[3] = tmp_lower[3]*xr[3];
      lowerProduct[4] = tmp_lower[4]*xr[4];
      lowerProduct[5] = tmp_lower[5]*xr[5];
      lowerProduct[6] = tmp_lower[6]*xr[6];
      lowerProduct[7] = tmp_lower[7]*xr[7];
      // Handle lower part
      Apsi[iNei0] += lowerProduct[0];
      Apsi[iNei1] += lowerProduct[1];
      Apsi[iNei2] += lowerProduct[2];
      Apsi[iNei3] += lowerProduct[3];
      Apsi[iNei4] += lowerProduct[4];
      Apsi[iNei5] += lowerProduct[5];
      Apsi[iNei6] += lowerProduct[6];
      Apsi[iNei7] += lowerProduct[7];
      // Increase the comunter
      *cnt = *cnt + 8;
    };
    Apsi[rid0] += upperProduct[0];
    Apsi[rid1] += upperProduct[1];
    Apsi[rid2] += upperProduct[2];
    Apsi[rid3] += upperProduct[3];
    Apsi[rid4] += upperProduct[4];
    Apsi[rid5] += upperProduct[5];
    Apsi[rid6] += upperProduct[6];
    Apsi[rid7] += upperProduct[7];
  };
  // printf( "exit AVX512\n" );
  // Exit point
  return;
};


template<const int NNZ>
inline void
product_sELL_AVX256
(
        idx_k*         const                      cnt,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        idx_k          const * const __restrict__ neigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  // printf( "AVX256 kernel: %10lu, %10lu, %10lu\n", NNZ, rlo, rhi );
  idx_k iRow;
  idx_k iCol;

  idx_k iNei0;
  idx_k iNei1;
  idx_k iNei2;
  idx_k iNei3;

  fld_k xr[4];
  fld_k tmp_psi[4];

  fld_k tmp_upper[4];
  fld_k tmp_lower[4];
  fld_k upperProduct[4];
  fld_k lowerProduct[4];

  for ( iRow=rlo; iRow<=rhi; iRow=iRow+4 ) // Loop over the rows
  {
    // row indices for the AVX rows
    const idx_k rid0 = iRow+0;
    const idx_k rid1 = iRow+1;
    const idx_k rid2 = iRow+2;
    const idx_k rid3 = iRow+3;
    // Packing right data
    xr[0] = psi[rid0];
    xr[1] = psi[rid1];
    xr[2] = psi[rid2];
    xr[3] = psi[rid3];
    // Initialize psi AVX buffer
    tmp_psi[0] = 0.0;
    tmp_psi[1] = 0.0;
    tmp_psi[2] = 0.0;
    tmp_psi[3] = 0.0;
    // Initialize tmp_upper AVX buffer
    tmp_upper[0] = 0.0;
    tmp_upper[1] = 0.0;
    tmp_upper[2] = 0.0;
    tmp_upper[3] = 0.0;
    // Initialize tmp_lower AVX buffer
    tmp_lower[0] = 0.0;
    tmp_lower[1] = 0.0;
    tmp_lower[2] = 0.0;
    tmp_lower[3] = 0.0;
    // Initialize upperProduct AVX buffer
    upperProduct[0] = 0.0;
    upperProduct[1] = 0.0;
    upperProduct[2] = 0.0;
    upperProduct[3] = 0.0;
    // Initialize neighbour indices AVX buffer
    // iNei0 = clo;
    // iNei1 = clo;
    // iNei2 = clo;
    // iNei3 = clo;
    // Loop over the non-zeroes elements in the lines
    for ( iCol=0; iCol<NNZ; ++iCol)
    {
      // Constructing indices
      const idx_k cid0 = *cnt+0;
      const idx_k cid1 = *cnt+1;
      const idx_k cid2 = *cnt+2;
      const idx_k cid3 = *cnt+3;
      // Handling indices
      iNei0 = neigh[cid0];
      iNei1 = neigh[cid1];
      iNei2 = neigh[cid2];
      iNei3 = neigh[cid3];
      // packing psi data
      tmp_psi[0] = psi[iNei0];
      tmp_psi[1] = psi[iNei1];
      tmp_psi[2] = psi[iNei2];
      tmp_psi[3] = psi[iNei3];
      // packing upper data
      tmp_upper[0] = upper[cid0];
      tmp_upper[1] = upper[cid1];
      tmp_upper[2] = upper[cid2];
      tmp_upper[3] = upper[cid3];
      // packing lower data
      tmp_lower[0] = lower[cid0];
      tmp_lower[1] = lower[cid1];
      tmp_lower[2] = lower[cid2];
      tmp_lower[3] = lower[cid3];
      // Compute upper FMS operation
      upperProduct[0] += tmp_upper[0]*tmp_psi[0];
      upperProduct[1] += tmp_upper[1]*tmp_psi[1];
      upperProduct[2] += tmp_upper[2]*tmp_psi[2];
      upperProduct[3] += tmp_upper[3]*tmp_psi[3];
      // Compute the lower product
      lowerProduct[0] = tmp_lower[0]*xr[0];
      lowerProduct[1] = tmp_lower[1]*xr[1];
      lowerProduct[2] = tmp_lower[2]*xr[2];
      lowerProduct[3] = tmp_lower[3]*xr[3];
      // Handle lower part
      Apsi[iNei0] += lowerProduct[0];
      Apsi[iNei1] += lowerProduct[1];
      Apsi[iNei2] += lowerProduct[2];
      Apsi[iNei3] += lowerProduct[3];
      // Increase the comunter
      *cnt = *cnt + 4;
    };
    Apsi[rid0] += upperProduct[0];
    Apsi[rid1] += upperProduct[1];
    Apsi[rid2] += upperProduct[2];
    Apsi[rid3] += upperProduct[3];
  };
  // printf( "exit AVX256\n" );
  // Exit point
  return;
};

template<const int NNZ>
inline void
product_sELL_AVX128
(
        idx_k*         const                      cnt,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        idx_k          const * const __restrict__ neigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  // printf( "AVX128 kernel: %10lu, %10lu, %10lu\n", NNZ, rlo, rhi );
  idx_k iRow;
  idx_k iCol;
  idx_k iNei0;
  idx_k iNei1;

  fld_k xr[2];
  fld_k tmp_psi[2];

  fld_k tmp_upper[2];
  fld_k tmp_lower[2];
  fld_k upperProduct[2];
  fld_k lowerProduct[2];

  for ( iRow=rlo; iRow<=rhi; iRow=iRow+2 ) // Loop over the rows
  {
    // row indices for the AVX rows
    const idx_k rid0 = iRow+0;
    const idx_k rid1 = iRow+1;
    // Packing right data
    xr[0] = psi[rid0];
    xr[1] = psi[rid1];
    // Initialize psi AVX buffer
    tmp_psi[0] = 0.0;
    tmp_psi[1] = 0.0;
    // Initialize tmp_upper AVX buffer
    tmp_upper[0] = 0.0;
    tmp_upper[1] = 0.0;
    // Initialize tmp_lower AVX buffer
    tmp_lower[0] = 0.0;
    tmp_lower[1] = 0.0;
    // Initialize upperProduct AVX buffer
    upperProduct[0] = 0.0;
    upperProduct[1] = 0.0;
    // Initialize neighbour indices AVX buffer
    // iNei0 = clo;
    // iNei1 = clo;
    // Loop over the non-zeroes elements in the lines
    for ( iCol=0; iCol<NNZ; ++iCol)
    {
      // Constructing indices
      const idx_k cid0 = *cnt+0;
      const idx_k cid1 = *cnt+1;
      // Handling indices
      iNei0 = neigh[cid0];
      iNei1 = neigh[cid1];
      // packing psi data
      tmp_psi[0] = psi[iNei0];
      tmp_psi[1] = psi[iNei1];
      // packing upper data
      tmp_upper[0] = upper[cid0];
      tmp_upper[1] = upper[cid1];
      // packing lower data
      tmp_lower[0] = lower[cid0];
      tmp_lower[1] = lower[cid1];
      // Compute upper FMS operation
      upperProduct[0] += tmp_upper[0]*tmp_psi[0];
      upperProduct[1] += tmp_upper[1]*tmp_psi[1];
      // Compute the lower product
      lowerProduct[0] = tmp_lower[0]*xr[0];
      lowerProduct[1] = tmp_lower[1]*xr[1];
      // Handle lower part
      Apsi[iNei0] += lowerProduct[0];
      Apsi[iNei1] += lowerProduct[1];
      // Increase the comunter
      *cnt = *cnt + 2;
    };
    Apsi[rid0] += upperProduct[0];
    Apsi[rid1] += upperProduct[1];
  };
  // printf( "exit AVX128\n" );
  // Exit point
  return;
};



template<const int NNZ>
inline void
product_sELL_AVX064
(
        idx_k*         const                      cnt,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        idx_k          const * const __restrict__ neigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  // printf( "AVX064 kernel: %10lu, %10lu, %10lu\n", NNZ, rlo, rhi );
  idx_k iRow;
  idx_k iCol;

  idx_k iNei0;

  fld_k xr;
  fld_k tmp_psi;

  fld_k tmp_upper;
  fld_k tmp_lower;
  fld_k upperProduct;
  fld_k lowerProduct;

  for ( iRow=rlo; iRow<=rhi; iRow++ ) // Loop over the rows
  {
    // row indices for the AVX rows
    const idx_k rid0 = iRow;
    // Packing right data
    xr = psi[rid0];
    // Initialize psi AVX buffer
    tmp_psi = 0.0;
    // Initialize tmp_upper AVX buffer
    tmp_upper = 0.0;
    // Initialize tmp_lower AVX buffer
    tmp_lower = 0.0;
    // Initialize upperProduct AVX buffer
    upperProduct = 0.0;
    // Initialize neighbour indices AVX buffer
    // iNei0 = clo;
    // Loop over the non-zeroes elements in the lines
    for ( iCol=0; iCol<NNZ; ++iCol)
    {
      // Constructing indices
      const idx_k cid0 = *cnt;
      // Handling indices
      iNei0 = neigh[cid0];
      // packing psi data
      tmp_psi = psi[iNei0];
      // packing upper data
      tmp_upper = upper[cid0];
      // packing lower data
      tmp_lower = lower[cid0];
      // Compute upper FMS operation
      upperProduct += tmp_upper*tmp_psi;
      // Compute the lower product
      lowerProduct = tmp_lower*xr;
      // Handle lower part
      Apsi[iNei0] += lowerProduct;
      // Increase the comunter
      *cnt = *cnt + 1;
    };
    Apsi[rid0] += upperProduct;
  };
  // printf( "exit AVX064\n" );
  // Exit point
  return;
};

static auto product001_sELL_AVX064 = product_sELL_AVX064<1>;
static auto product002_sELL_AVX064 = product_sELL_AVX064<2>;
static auto product003_sELL_AVX064 = product_sELL_AVX064<3>;
static auto product004_sELL_AVX064 = product_sELL_AVX064<4>;
static auto product005_sELL_AVX064 = product_sELL_AVX064<5>;
static auto product006_sELL_AVX064 = product_sELL_AVX064<6>;
static auto product007_sELL_AVX064 = product_sELL_AVX064<7>;
static auto product008_sELL_AVX064 = product_sELL_AVX064<8>;
static auto product009_sELL_AVX064 = product_sELL_AVX064<9>;
static auto product010_sELL_AVX064 = product_sELL_AVX064<10>;
static auto product011_sELL_AVX064 = product_sELL_AVX064<11>;
static auto product012_sELL_AVX064 = product_sELL_AVX064<12>;
static auto product013_sELL_AVX064 = product_sELL_AVX064<13>;
static auto product014_sELL_AVX064 = product_sELL_AVX064<14>;
static auto product015_sELL_AVX064 = product_sELL_AVX064<15>;
static auto product016_sELL_AVX064 = product_sELL_AVX064<16>;
static auto product017_sELL_AVX064 = product_sELL_AVX064<17>;
static auto product018_sELL_AVX064 = product_sELL_AVX064<18>;
static auto product019_sELL_AVX064 = product_sELL_AVX064<19>;
static auto product020_sELL_AVX064 = product_sELL_AVX064<20>;
static auto product021_sELL_AVX064 = product_sELL_AVX064<21>;
static auto product022_sELL_AVX064 = product_sELL_AVX064<22>;
static auto product023_sELL_AVX064 = product_sELL_AVX064<23>;
static auto product024_sELL_AVX064 = product_sELL_AVX064<24>;
static auto product025_sELL_AVX064 = product_sELL_AVX064<25>;
static auto product026_sELL_AVX064 = product_sELL_AVX064<26>;
static auto product027_sELL_AVX064 = product_sELL_AVX064<27>;
static auto product028_sELL_AVX064 = product_sELL_AVX064<28>;
static auto product029_sELL_AVX064 = product_sELL_AVX064<29>;
static auto product030_sELL_AVX064 = product_sELL_AVX064<30>;
static auto product031_sELL_AVX064 = product_sELL_AVX064<31>;
static auto product032_sELL_AVX064 = product_sELL_AVX064<32>;
static auto product033_sELL_AVX064 = product_sELL_AVX064<33>;
static auto product034_sELL_AVX064 = product_sELL_AVX064<34>;
static auto product035_sELL_AVX064 = product_sELL_AVX064<35>;
static auto product036_sELL_AVX064 = product_sELL_AVX064<36>;
static auto product037_sELL_AVX064 = product_sELL_AVX064<37>;
static auto product038_sELL_AVX064 = product_sELL_AVX064<38>;
static auto product039_sELL_AVX064 = product_sELL_AVX064<39>;
static auto product040_sELL_AVX064 = product_sELL_AVX064<40>;
static auto product041_sELL_AVX064 = product_sELL_AVX064<41>;
static auto product042_sELL_AVX064 = product_sELL_AVX064<42>;
static auto product043_sELL_AVX064 = product_sELL_AVX064<43>;
static auto product044_sELL_AVX064 = product_sELL_AVX064<44>;
static auto product045_sELL_AVX064 = product_sELL_AVX064<45>;
static auto product046_sELL_AVX064 = product_sELL_AVX064<46>;
static auto product047_sELL_AVX064 = product_sELL_AVX064<47>;
static auto product048_sELL_AVX064 = product_sELL_AVX064<48>;
static auto product049_sELL_AVX064 = product_sELL_AVX064<49>;
static auto product050_sELL_AVX064 = product_sELL_AVX064<50>;
static auto product051_sELL_AVX064 = product_sELL_AVX064<51>;
static auto product052_sELL_AVX064 = product_sELL_AVX064<52>;
static auto product053_sELL_AVX064 = product_sELL_AVX064<53>;
static auto product054_sELL_AVX064 = product_sELL_AVX064<54>;
static auto product055_sELL_AVX064 = product_sELL_AVX064<55>;
static auto product056_sELL_AVX064 = product_sELL_AVX064<56>;
static auto product057_sELL_AVX064 = product_sELL_AVX064<57>;
static auto product058_sELL_AVX064 = product_sELL_AVX064<58>;
static auto product059_sELL_AVX064 = product_sELL_AVX064<59>;
static auto product060_sELL_AVX064 = product_sELL_AVX064<60>;
static auto product061_sELL_AVX064 = product_sELL_AVX064<61>;
static auto product062_sELL_AVX064 = product_sELL_AVX064<62>;
static auto product063_sELL_AVX064 = product_sELL_AVX064<63>;
static auto product064_sELL_AVX064 = product_sELL_AVX064<64>;
static auto product065_sELL_AVX064 = product_sELL_AVX064<65>;
static auto product066_sELL_AVX064 = product_sELL_AVX064<66>;
static auto product067_sELL_AVX064 = product_sELL_AVX064<67>;
static auto product068_sELL_AVX064 = product_sELL_AVX064<68>;
static auto product069_sELL_AVX064 = product_sELL_AVX064<69>;
static auto product070_sELL_AVX064 = product_sELL_AVX064<70>;
static auto product071_sELL_AVX064 = product_sELL_AVX064<71>;
static auto product072_sELL_AVX064 = product_sELL_AVX064<72>;
static auto product073_sELL_AVX064 = product_sELL_AVX064<73>;
static auto product074_sELL_AVX064 = product_sELL_AVX064<74>;
static auto product075_sELL_AVX064 = product_sELL_AVX064<75>;
static auto product076_sELL_AVX064 = product_sELL_AVX064<76>;
static auto product077_sELL_AVX064 = product_sELL_AVX064<77>;
static auto product078_sELL_AVX064 = product_sELL_AVX064<78>;
static auto product079_sELL_AVX064 = product_sELL_AVX064<79>;
static auto product080_sELL_AVX064 = product_sELL_AVX064<80>;
static auto product081_sELL_AVX064 = product_sELL_AVX064<81>;
static auto product082_sELL_AVX064 = product_sELL_AVX064<82>;
static auto product083_sELL_AVX064 = product_sELL_AVX064<83>;
static auto product084_sELL_AVX064 = product_sELL_AVX064<84>;
static auto product085_sELL_AVX064 = product_sELL_AVX064<85>;
static auto product086_sELL_AVX064 = product_sELL_AVX064<86>;
static auto product087_sELL_AVX064 = product_sELL_AVX064<87>;
static auto product088_sELL_AVX064 = product_sELL_AVX064<88>;
static auto product089_sELL_AVX064 = product_sELL_AVX064<89>;
static auto product090_sELL_AVX064 = product_sELL_AVX064<90>;
static auto product091_sELL_AVX064 = product_sELL_AVX064<91>;
static auto product092_sELL_AVX064 = product_sELL_AVX064<92>;
static auto product093_sELL_AVX064 = product_sELL_AVX064<93>;
static auto product094_sELL_AVX064 = product_sELL_AVX064<94>;
static auto product095_sELL_AVX064 = product_sELL_AVX064<95>;
static auto product096_sELL_AVX064 = product_sELL_AVX064<96>;
static auto product097_sELL_AVX064 = product_sELL_AVX064<97>;
static auto product098_sELL_AVX064 = product_sELL_AVX064<98>;
static auto product099_sELL_AVX064 = product_sELL_AVX064<99>;
static auto product100_sELL_AVX064 = product_sELL_AVX064<100>;
static auto product101_sELL_AVX064 = product_sELL_AVX064<101>;
static auto product102_sELL_AVX064 = product_sELL_AVX064<102>;
static auto product103_sELL_AVX064 = product_sELL_AVX064<103>;
static auto product104_sELL_AVX064 = product_sELL_AVX064<104>;
static auto product105_sELL_AVX064 = product_sELL_AVX064<105>;
static auto product106_sELL_AVX064 = product_sELL_AVX064<106>;
static auto product107_sELL_AVX064 = product_sELL_AVX064<107>;
static auto product108_sELL_AVX064 = product_sELL_AVX064<108>;
static auto product109_sELL_AVX064 = product_sELL_AVX064<109>;
static auto product110_sELL_AVX064 = product_sELL_AVX064<110>;
static auto product111_sELL_AVX064 = product_sELL_AVX064<111>;
static auto product112_sELL_AVX064 = product_sELL_AVX064<112>;
static auto product113_sELL_AVX064 = product_sELL_AVX064<113>;
static auto product114_sELL_AVX064 = product_sELL_AVX064<114>;
static auto product115_sELL_AVX064 = product_sELL_AVX064<115>;
static auto product116_sELL_AVX064 = product_sELL_AVX064<116>;
static auto product117_sELL_AVX064 = product_sELL_AVX064<117>;
static auto product118_sELL_AVX064 = product_sELL_AVX064<118>;
static auto product119_sELL_AVX064 = product_sELL_AVX064<119>;
static auto product120_sELL_AVX064 = product_sELL_AVX064<120>;
static auto product121_sELL_AVX064 = product_sELL_AVX064<121>;
static auto product122_sELL_AVX064 = product_sELL_AVX064<122>;
static auto product123_sELL_AVX064 = product_sELL_AVX064<123>;
static auto product124_sELL_AVX064 = product_sELL_AVX064<124>;
static auto product125_sELL_AVX064 = product_sELL_AVX064<125>;
static auto product126_sELL_AVX064 = product_sELL_AVX064<126>;
static auto product127_sELL_AVX064 = product_sELL_AVX064<127>;
static auto product128_sELL_AVX064 = product_sELL_AVX064<128>;
static auto product001_sELL_AVX128 = product_sELL_AVX128<1>;
static auto product002_sELL_AVX128 = product_sELL_AVX128<2>;
static auto product003_sELL_AVX128 = product_sELL_AVX128<3>;
static auto product004_sELL_AVX128 = product_sELL_AVX128<4>;
static auto product005_sELL_AVX128 = product_sELL_AVX128<5>;
static auto product006_sELL_AVX128 = product_sELL_AVX128<6>;
static auto product007_sELL_AVX128 = product_sELL_AVX128<7>;
static auto product008_sELL_AVX128 = product_sELL_AVX128<8>;
static auto product009_sELL_AVX128 = product_sELL_AVX128<9>;
static auto product010_sELL_AVX128 = product_sELL_AVX128<10>;
static auto product011_sELL_AVX128 = product_sELL_AVX128<11>;
static auto product012_sELL_AVX128 = product_sELL_AVX128<12>;
static auto product013_sELL_AVX128 = product_sELL_AVX128<13>;
static auto product014_sELL_AVX128 = product_sELL_AVX128<14>;
static auto product015_sELL_AVX128 = product_sELL_AVX128<15>;
static auto product016_sELL_AVX128 = product_sELL_AVX128<16>;
static auto product017_sELL_AVX128 = product_sELL_AVX128<17>;
static auto product018_sELL_AVX128 = product_sELL_AVX128<18>;
static auto product019_sELL_AVX128 = product_sELL_AVX128<19>;
static auto product020_sELL_AVX128 = product_sELL_AVX128<20>;
static auto product021_sELL_AVX128 = product_sELL_AVX128<21>;
static auto product022_sELL_AVX128 = product_sELL_AVX128<22>;
static auto product023_sELL_AVX128 = product_sELL_AVX128<23>;
static auto product024_sELL_AVX128 = product_sELL_AVX128<24>;
static auto product025_sELL_AVX128 = product_sELL_AVX128<25>;
static auto product026_sELL_AVX128 = product_sELL_AVX128<26>;
static auto product027_sELL_AVX128 = product_sELL_AVX128<27>;
static auto product028_sELL_AVX128 = product_sELL_AVX128<28>;
static auto product029_sELL_AVX128 = product_sELL_AVX128<29>;
static auto product030_sELL_AVX128 = product_sELL_AVX128<30>;
static auto product031_sELL_AVX128 = product_sELL_AVX128<31>;
static auto product032_sELL_AVX128 = product_sELL_AVX128<32>;
static auto product033_sELL_AVX128 = product_sELL_AVX128<33>;
static auto product034_sELL_AVX128 = product_sELL_AVX128<34>;
static auto product035_sELL_AVX128 = product_sELL_AVX128<35>;
static auto product036_sELL_AVX128 = product_sELL_AVX128<36>;
static auto product037_sELL_AVX128 = product_sELL_AVX128<37>;
static auto product038_sELL_AVX128 = product_sELL_AVX128<38>;
static auto product039_sELL_AVX128 = product_sELL_AVX128<39>;
static auto product040_sELL_AVX128 = product_sELL_AVX128<40>;
static auto product041_sELL_AVX128 = product_sELL_AVX128<41>;
static auto product042_sELL_AVX128 = product_sELL_AVX128<42>;
static auto product043_sELL_AVX128 = product_sELL_AVX128<43>;
static auto product044_sELL_AVX128 = product_sELL_AVX128<44>;
static auto product045_sELL_AVX128 = product_sELL_AVX128<45>;
static auto product046_sELL_AVX128 = product_sELL_AVX128<46>;
static auto product047_sELL_AVX128 = product_sELL_AVX128<47>;
static auto product048_sELL_AVX128 = product_sELL_AVX128<48>;
static auto product049_sELL_AVX128 = product_sELL_AVX128<49>;
static auto product050_sELL_AVX128 = product_sELL_AVX128<50>;
static auto product051_sELL_AVX128 = product_sELL_AVX128<51>;
static auto product052_sELL_AVX128 = product_sELL_AVX128<52>;
static auto product053_sELL_AVX128 = product_sELL_AVX128<53>;
static auto product054_sELL_AVX128 = product_sELL_AVX128<54>;
static auto product055_sELL_AVX128 = product_sELL_AVX128<55>;
static auto product056_sELL_AVX128 = product_sELL_AVX128<56>;
static auto product057_sELL_AVX128 = product_sELL_AVX128<57>;
static auto product058_sELL_AVX128 = product_sELL_AVX128<58>;
static auto product059_sELL_AVX128 = product_sELL_AVX128<59>;
static auto product060_sELL_AVX128 = product_sELL_AVX128<60>;
static auto product061_sELL_AVX128 = product_sELL_AVX128<61>;
static auto product062_sELL_AVX128 = product_sELL_AVX128<62>;
static auto product063_sELL_AVX128 = product_sELL_AVX128<63>;
static auto product064_sELL_AVX128 = product_sELL_AVX128<64>;
static auto product065_sELL_AVX128 = product_sELL_AVX128<65>;
static auto product066_sELL_AVX128 = product_sELL_AVX128<66>;
static auto product067_sELL_AVX128 = product_sELL_AVX128<67>;
static auto product068_sELL_AVX128 = product_sELL_AVX128<68>;
static auto product069_sELL_AVX128 = product_sELL_AVX128<69>;
static auto product070_sELL_AVX128 = product_sELL_AVX128<70>;
static auto product071_sELL_AVX128 = product_sELL_AVX128<71>;
static auto product072_sELL_AVX128 = product_sELL_AVX128<72>;
static auto product073_sELL_AVX128 = product_sELL_AVX128<73>;
static auto product074_sELL_AVX128 = product_sELL_AVX128<74>;
static auto product075_sELL_AVX128 = product_sELL_AVX128<75>;
static auto product076_sELL_AVX128 = product_sELL_AVX128<76>;
static auto product077_sELL_AVX128 = product_sELL_AVX128<77>;
static auto product078_sELL_AVX128 = product_sELL_AVX128<78>;
static auto product079_sELL_AVX128 = product_sELL_AVX128<79>;
static auto product080_sELL_AVX128 = product_sELL_AVX128<80>;
static auto product081_sELL_AVX128 = product_sELL_AVX128<81>;
static auto product082_sELL_AVX128 = product_sELL_AVX128<82>;
static auto product083_sELL_AVX128 = product_sELL_AVX128<83>;
static auto product084_sELL_AVX128 = product_sELL_AVX128<84>;
static auto product085_sELL_AVX128 = product_sELL_AVX128<85>;
static auto product086_sELL_AVX128 = product_sELL_AVX128<86>;
static auto product087_sELL_AVX128 = product_sELL_AVX128<87>;
static auto product088_sELL_AVX128 = product_sELL_AVX128<88>;
static auto product089_sELL_AVX128 = product_sELL_AVX128<89>;
static auto product090_sELL_AVX128 = product_sELL_AVX128<90>;
static auto product091_sELL_AVX128 = product_sELL_AVX128<91>;
static auto product092_sELL_AVX128 = product_sELL_AVX128<92>;
static auto product093_sELL_AVX128 = product_sELL_AVX128<93>;
static auto product094_sELL_AVX128 = product_sELL_AVX128<94>;
static auto product095_sELL_AVX128 = product_sELL_AVX128<95>;
static auto product096_sELL_AVX128 = product_sELL_AVX128<96>;
static auto product097_sELL_AVX128 = product_sELL_AVX128<97>;
static auto product098_sELL_AVX128 = product_sELL_AVX128<98>;
static auto product099_sELL_AVX128 = product_sELL_AVX128<99>;
static auto product100_sELL_AVX128 = product_sELL_AVX128<100>;
static auto product101_sELL_AVX128 = product_sELL_AVX128<101>;
static auto product102_sELL_AVX128 = product_sELL_AVX128<102>;
static auto product103_sELL_AVX128 = product_sELL_AVX128<103>;
static auto product104_sELL_AVX128 = product_sELL_AVX128<104>;
static auto product105_sELL_AVX128 = product_sELL_AVX128<105>;
static auto product106_sELL_AVX128 = product_sELL_AVX128<106>;
static auto product107_sELL_AVX128 = product_sELL_AVX128<107>;
static auto product108_sELL_AVX128 = product_sELL_AVX128<108>;
static auto product109_sELL_AVX128 = product_sELL_AVX128<109>;
static auto product110_sELL_AVX128 = product_sELL_AVX128<110>;
static auto product111_sELL_AVX128 = product_sELL_AVX128<111>;
static auto product112_sELL_AVX128 = product_sELL_AVX128<112>;
static auto product113_sELL_AVX128 = product_sELL_AVX128<113>;
static auto product114_sELL_AVX128 = product_sELL_AVX128<114>;
static auto product115_sELL_AVX128 = product_sELL_AVX128<115>;
static auto product116_sELL_AVX128 = product_sELL_AVX128<116>;
static auto product117_sELL_AVX128 = product_sELL_AVX128<117>;
static auto product118_sELL_AVX128 = product_sELL_AVX128<118>;
static auto product119_sELL_AVX128 = product_sELL_AVX128<119>;
static auto product120_sELL_AVX128 = product_sELL_AVX128<120>;
static auto product121_sELL_AVX128 = product_sELL_AVX128<121>;
static auto product122_sELL_AVX128 = product_sELL_AVX128<122>;
static auto product123_sELL_AVX128 = product_sELL_AVX128<123>;
static auto product124_sELL_AVX128 = product_sELL_AVX128<124>;
static auto product125_sELL_AVX128 = product_sELL_AVX128<125>;
static auto product126_sELL_AVX128 = product_sELL_AVX128<126>;
static auto product127_sELL_AVX128 = product_sELL_AVX128<127>;
static auto product128_sELL_AVX128 = product_sELL_AVX128<128>;
static auto product001_sELL_AVX256 = product_sELL_AVX256<1>;
static auto product002_sELL_AVX256 = product_sELL_AVX256<2>;
static auto product003_sELL_AVX256 = product_sELL_AVX256<3>;
static auto product004_sELL_AVX256 = product_sELL_AVX256<4>;
static auto product005_sELL_AVX256 = product_sELL_AVX256<5>;
static auto product006_sELL_AVX256 = product_sELL_AVX256<6>;
static auto product007_sELL_AVX256 = product_sELL_AVX256<7>;
static auto product008_sELL_AVX256 = product_sELL_AVX256<8>;
static auto product009_sELL_AVX256 = product_sELL_AVX256<9>;
static auto product010_sELL_AVX256 = product_sELL_AVX256<10>;
static auto product011_sELL_AVX256 = product_sELL_AVX256<11>;
static auto product012_sELL_AVX256 = product_sELL_AVX256<12>;
static auto product013_sELL_AVX256 = product_sELL_AVX256<13>;
static auto product014_sELL_AVX256 = product_sELL_AVX256<14>;
static auto product015_sELL_AVX256 = product_sELL_AVX256<15>;
static auto product016_sELL_AVX256 = product_sELL_AVX256<16>;
static auto product017_sELL_AVX256 = product_sELL_AVX256<17>;
static auto product018_sELL_AVX256 = product_sELL_AVX256<18>;
static auto product019_sELL_AVX256 = product_sELL_AVX256<19>;
static auto product020_sELL_AVX256 = product_sELL_AVX256<20>;
static auto product021_sELL_AVX256 = product_sELL_AVX256<21>;
static auto product022_sELL_AVX256 = product_sELL_AVX256<22>;
static auto product023_sELL_AVX256 = product_sELL_AVX256<23>;
static auto product024_sELL_AVX256 = product_sELL_AVX256<24>;
static auto product025_sELL_AVX256 = product_sELL_AVX256<25>;
static auto product026_sELL_AVX256 = product_sELL_AVX256<26>;
static auto product027_sELL_AVX256 = product_sELL_AVX256<27>;
static auto product028_sELL_AVX256 = product_sELL_AVX256<28>;
static auto product029_sELL_AVX256 = product_sELL_AVX256<29>;
static auto product030_sELL_AVX256 = product_sELL_AVX256<30>;
static auto product031_sELL_AVX256 = product_sELL_AVX256<31>;
static auto product032_sELL_AVX256 = product_sELL_AVX256<32>;
static auto product033_sELL_AVX256 = product_sELL_AVX256<33>;
static auto product034_sELL_AVX256 = product_sELL_AVX256<34>;
static auto product035_sELL_AVX256 = product_sELL_AVX256<35>;
static auto product036_sELL_AVX256 = product_sELL_AVX256<36>;
static auto product037_sELL_AVX256 = product_sELL_AVX256<37>;
static auto product038_sELL_AVX256 = product_sELL_AVX256<38>;
static auto product039_sELL_AVX256 = product_sELL_AVX256<39>;
static auto product040_sELL_AVX256 = product_sELL_AVX256<40>;
static auto product041_sELL_AVX256 = product_sELL_AVX256<41>;
static auto product042_sELL_AVX256 = product_sELL_AVX256<42>;
static auto product043_sELL_AVX256 = product_sELL_AVX256<43>;
static auto product044_sELL_AVX256 = product_sELL_AVX256<44>;
static auto product045_sELL_AVX256 = product_sELL_AVX256<45>;
static auto product046_sELL_AVX256 = product_sELL_AVX256<46>;
static auto product047_sELL_AVX256 = product_sELL_AVX256<47>;
static auto product048_sELL_AVX256 = product_sELL_AVX256<48>;
static auto product049_sELL_AVX256 = product_sELL_AVX256<49>;
static auto product050_sELL_AVX256 = product_sELL_AVX256<50>;
static auto product051_sELL_AVX256 = product_sELL_AVX256<51>;
static auto product052_sELL_AVX256 = product_sELL_AVX256<52>;
static auto product053_sELL_AVX256 = product_sELL_AVX256<53>;
static auto product054_sELL_AVX256 = product_sELL_AVX256<54>;
static auto product055_sELL_AVX256 = product_sELL_AVX256<55>;
static auto product056_sELL_AVX256 = product_sELL_AVX256<56>;
static auto product057_sELL_AVX256 = product_sELL_AVX256<57>;
static auto product058_sELL_AVX256 = product_sELL_AVX256<58>;
static auto product059_sELL_AVX256 = product_sELL_AVX256<59>;
static auto product060_sELL_AVX256 = product_sELL_AVX256<60>;
static auto product061_sELL_AVX256 = product_sELL_AVX256<61>;
static auto product062_sELL_AVX256 = product_sELL_AVX256<62>;
static auto product063_sELL_AVX256 = product_sELL_AVX256<63>;
static auto product064_sELL_AVX256 = product_sELL_AVX256<64>;
static auto product065_sELL_AVX256 = product_sELL_AVX256<65>;
static auto product066_sELL_AVX256 = product_sELL_AVX256<66>;
static auto product067_sELL_AVX256 = product_sELL_AVX256<67>;
static auto product068_sELL_AVX256 = product_sELL_AVX256<68>;
static auto product069_sELL_AVX256 = product_sELL_AVX256<69>;
static auto product070_sELL_AVX256 = product_sELL_AVX256<70>;
static auto product071_sELL_AVX256 = product_sELL_AVX256<71>;
static auto product072_sELL_AVX256 = product_sELL_AVX256<72>;
static auto product073_sELL_AVX256 = product_sELL_AVX256<73>;
static auto product074_sELL_AVX256 = product_sELL_AVX256<74>;
static auto product075_sELL_AVX256 = product_sELL_AVX256<75>;
static auto product076_sELL_AVX256 = product_sELL_AVX256<76>;
static auto product077_sELL_AVX256 = product_sELL_AVX256<77>;
static auto product078_sELL_AVX256 = product_sELL_AVX256<78>;
static auto product079_sELL_AVX256 = product_sELL_AVX256<79>;
static auto product080_sELL_AVX256 = product_sELL_AVX256<80>;
static auto product081_sELL_AVX256 = product_sELL_AVX256<81>;
static auto product082_sELL_AVX256 = product_sELL_AVX256<82>;
static auto product083_sELL_AVX256 = product_sELL_AVX256<83>;
static auto product084_sELL_AVX256 = product_sELL_AVX256<84>;
static auto product085_sELL_AVX256 = product_sELL_AVX256<85>;
static auto product086_sELL_AVX256 = product_sELL_AVX256<86>;
static auto product087_sELL_AVX256 = product_sELL_AVX256<87>;
static auto product088_sELL_AVX256 = product_sELL_AVX256<88>;
static auto product089_sELL_AVX256 = product_sELL_AVX256<89>;
static auto product090_sELL_AVX256 = product_sELL_AVX256<90>;
static auto product091_sELL_AVX256 = product_sELL_AVX256<91>;
static auto product092_sELL_AVX256 = product_sELL_AVX256<92>;
static auto product093_sELL_AVX256 = product_sELL_AVX256<93>;
static auto product094_sELL_AVX256 = product_sELL_AVX256<94>;
static auto product095_sELL_AVX256 = product_sELL_AVX256<95>;
static auto product096_sELL_AVX256 = product_sELL_AVX256<96>;
static auto product097_sELL_AVX256 = product_sELL_AVX256<97>;
static auto product098_sELL_AVX256 = product_sELL_AVX256<98>;
static auto product099_sELL_AVX256 = product_sELL_AVX256<99>;
static auto product100_sELL_AVX256 = product_sELL_AVX256<100>;
static auto product101_sELL_AVX256 = product_sELL_AVX256<101>;
static auto product102_sELL_AVX256 = product_sELL_AVX256<102>;
static auto product103_sELL_AVX256 = product_sELL_AVX256<103>;
static auto product104_sELL_AVX256 = product_sELL_AVX256<104>;
static auto product105_sELL_AVX256 = product_sELL_AVX256<105>;
static auto product106_sELL_AVX256 = product_sELL_AVX256<106>;
static auto product107_sELL_AVX256 = product_sELL_AVX256<107>;
static auto product108_sELL_AVX256 = product_sELL_AVX256<108>;
static auto product109_sELL_AVX256 = product_sELL_AVX256<109>;
static auto product110_sELL_AVX256 = product_sELL_AVX256<110>;
static auto product111_sELL_AVX256 = product_sELL_AVX256<111>;
static auto product112_sELL_AVX256 = product_sELL_AVX256<112>;
static auto product113_sELL_AVX256 = product_sELL_AVX256<113>;
static auto product114_sELL_AVX256 = product_sELL_AVX256<114>;
static auto product115_sELL_AVX256 = product_sELL_AVX256<115>;
static auto product116_sELL_AVX256 = product_sELL_AVX256<116>;
static auto product117_sELL_AVX256 = product_sELL_AVX256<117>;
static auto product118_sELL_AVX256 = product_sELL_AVX256<118>;
static auto product119_sELL_AVX256 = product_sELL_AVX256<119>;
static auto product120_sELL_AVX256 = product_sELL_AVX256<120>;
static auto product121_sELL_AVX256 = product_sELL_AVX256<121>;
static auto product122_sELL_AVX256 = product_sELL_AVX256<122>;
static auto product123_sELL_AVX256 = product_sELL_AVX256<123>;
static auto product124_sELL_AVX256 = product_sELL_AVX256<124>;
static auto product125_sELL_AVX256 = product_sELL_AVX256<125>;
static auto product126_sELL_AVX256 = product_sELL_AVX256<126>;
static auto product127_sELL_AVX256 = product_sELL_AVX256<127>;
static auto product128_sELL_AVX256 = product_sELL_AVX256<128>;
static auto product001_sELL_AVX512 = product_sELL_AVX512<1>;
static auto product002_sELL_AVX512 = product_sELL_AVX512<2>;
static auto product003_sELL_AVX512 = product_sELL_AVX512<3>;
static auto product004_sELL_AVX512 = product_sELL_AVX512<4>;
static auto product005_sELL_AVX512 = product_sELL_AVX512<5>;
static auto product006_sELL_AVX512 = product_sELL_AVX512<6>;
static auto product007_sELL_AVX512 = product_sELL_AVX512<7>;
static auto product008_sELL_AVX512 = product_sELL_AVX512<8>;
static auto product009_sELL_AVX512 = product_sELL_AVX512<9>;
static auto product010_sELL_AVX512 = product_sELL_AVX512<10>;
static auto product011_sELL_AVX512 = product_sELL_AVX512<11>;
static auto product012_sELL_AVX512 = product_sELL_AVX512<12>;
static auto product013_sELL_AVX512 = product_sELL_AVX512<13>;
static auto product014_sELL_AVX512 = product_sELL_AVX512<14>;
static auto product015_sELL_AVX512 = product_sELL_AVX512<15>;
static auto product016_sELL_AVX512 = product_sELL_AVX512<16>;
static auto product017_sELL_AVX512 = product_sELL_AVX512<17>;
static auto product018_sELL_AVX512 = product_sELL_AVX512<18>;
static auto product019_sELL_AVX512 = product_sELL_AVX512<19>;
static auto product020_sELL_AVX512 = product_sELL_AVX512<20>;
static auto product021_sELL_AVX512 = product_sELL_AVX512<21>;
static auto product022_sELL_AVX512 = product_sELL_AVX512<22>;
static auto product023_sELL_AVX512 = product_sELL_AVX512<23>;
static auto product024_sELL_AVX512 = product_sELL_AVX512<24>;
static auto product025_sELL_AVX512 = product_sELL_AVX512<25>;
static auto product026_sELL_AVX512 = product_sELL_AVX512<26>;
static auto product027_sELL_AVX512 = product_sELL_AVX512<27>;
static auto product028_sELL_AVX512 = product_sELL_AVX512<28>;
static auto product029_sELL_AVX512 = product_sELL_AVX512<29>;
static auto product030_sELL_AVX512 = product_sELL_AVX512<30>;
static auto product031_sELL_AVX512 = product_sELL_AVX512<31>;
static auto product032_sELL_AVX512 = product_sELL_AVX512<32>;
static auto product033_sELL_AVX512 = product_sELL_AVX512<33>;
static auto product034_sELL_AVX512 = product_sELL_AVX512<34>;
static auto product035_sELL_AVX512 = product_sELL_AVX512<35>;
static auto product036_sELL_AVX512 = product_sELL_AVX512<36>;
static auto product037_sELL_AVX512 = product_sELL_AVX512<37>;
static auto product038_sELL_AVX512 = product_sELL_AVX512<38>;
static auto product039_sELL_AVX512 = product_sELL_AVX512<39>;
static auto product040_sELL_AVX512 = product_sELL_AVX512<40>;
static auto product041_sELL_AVX512 = product_sELL_AVX512<41>;
static auto product042_sELL_AVX512 = product_sELL_AVX512<42>;
static auto product043_sELL_AVX512 = product_sELL_AVX512<43>;
static auto product044_sELL_AVX512 = product_sELL_AVX512<44>;
static auto product045_sELL_AVX512 = product_sELL_AVX512<45>;
static auto product046_sELL_AVX512 = product_sELL_AVX512<46>;
static auto product047_sELL_AVX512 = product_sELL_AVX512<47>;
static auto product048_sELL_AVX512 = product_sELL_AVX512<48>;
static auto product049_sELL_AVX512 = product_sELL_AVX512<49>;
static auto product050_sELL_AVX512 = product_sELL_AVX512<50>;
static auto product051_sELL_AVX512 = product_sELL_AVX512<51>;
static auto product052_sELL_AVX512 = product_sELL_AVX512<52>;
static auto product053_sELL_AVX512 = product_sELL_AVX512<53>;
static auto product054_sELL_AVX512 = product_sELL_AVX512<54>;
static auto product055_sELL_AVX512 = product_sELL_AVX512<55>;
static auto product056_sELL_AVX512 = product_sELL_AVX512<56>;
static auto product057_sELL_AVX512 = product_sELL_AVX512<57>;
static auto product058_sELL_AVX512 = product_sELL_AVX512<58>;
static auto product059_sELL_AVX512 = product_sELL_AVX512<59>;
static auto product060_sELL_AVX512 = product_sELL_AVX512<60>;
static auto product061_sELL_AVX512 = product_sELL_AVX512<61>;
static auto product062_sELL_AVX512 = product_sELL_AVX512<62>;
static auto product063_sELL_AVX512 = product_sELL_AVX512<63>;
static auto product064_sELL_AVX512 = product_sELL_AVX512<64>;
static auto product065_sELL_AVX512 = product_sELL_AVX512<65>;
static auto product066_sELL_AVX512 = product_sELL_AVX512<66>;
static auto product067_sELL_AVX512 = product_sELL_AVX512<67>;
static auto product068_sELL_AVX512 = product_sELL_AVX512<68>;
static auto product069_sELL_AVX512 = product_sELL_AVX512<69>;
static auto product070_sELL_AVX512 = product_sELL_AVX512<70>;
static auto product071_sELL_AVX512 = product_sELL_AVX512<71>;
static auto product072_sELL_AVX512 = product_sELL_AVX512<72>;
static auto product073_sELL_AVX512 = product_sELL_AVX512<73>;
static auto product074_sELL_AVX512 = product_sELL_AVX512<74>;
static auto product075_sELL_AVX512 = product_sELL_AVX512<75>;
static auto product076_sELL_AVX512 = product_sELL_AVX512<76>;
static auto product077_sELL_AVX512 = product_sELL_AVX512<77>;
static auto product078_sELL_AVX512 = product_sELL_AVX512<78>;
static auto product079_sELL_AVX512 = product_sELL_AVX512<79>;
static auto product080_sELL_AVX512 = product_sELL_AVX512<80>;
static auto product081_sELL_AVX512 = product_sELL_AVX512<81>;
static auto product082_sELL_AVX512 = product_sELL_AVX512<82>;
static auto product083_sELL_AVX512 = product_sELL_AVX512<83>;
static auto product084_sELL_AVX512 = product_sELL_AVX512<84>;
static auto product085_sELL_AVX512 = product_sELL_AVX512<85>;
static auto product086_sELL_AVX512 = product_sELL_AVX512<86>;
static auto product087_sELL_AVX512 = product_sELL_AVX512<87>;
static auto product088_sELL_AVX512 = product_sELL_AVX512<88>;
static auto product089_sELL_AVX512 = product_sELL_AVX512<89>;
static auto product090_sELL_AVX512 = product_sELL_AVX512<90>;
static auto product091_sELL_AVX512 = product_sELL_AVX512<91>;
static auto product092_sELL_AVX512 = product_sELL_AVX512<92>;
static auto product093_sELL_AVX512 = product_sELL_AVX512<93>;
static auto product094_sELL_AVX512 = product_sELL_AVX512<94>;
static auto product095_sELL_AVX512 = product_sELL_AVX512<95>;
static auto product096_sELL_AVX512 = product_sELL_AVX512<96>;
static auto product097_sELL_AVX512 = product_sELL_AVX512<97>;
static auto product098_sELL_AVX512 = product_sELL_AVX512<98>;
static auto product099_sELL_AVX512 = product_sELL_AVX512<99>;
static auto product100_sELL_AVX512 = product_sELL_AVX512<100>;
static auto product101_sELL_AVX512 = product_sELL_AVX512<101>;
static auto product102_sELL_AVX512 = product_sELL_AVX512<102>;
static auto product103_sELL_AVX512 = product_sELL_AVX512<103>;
static auto product104_sELL_AVX512 = product_sELL_AVX512<104>;
static auto product105_sELL_AVX512 = product_sELL_AVX512<105>;
static auto product106_sELL_AVX512 = product_sELL_AVX512<106>;
static auto product107_sELL_AVX512 = product_sELL_AVX512<107>;
static auto product108_sELL_AVX512 = product_sELL_AVX512<108>;
static auto product109_sELL_AVX512 = product_sELL_AVX512<109>;
static auto product110_sELL_AVX512 = product_sELL_AVX512<110>;
static auto product111_sELL_AVX512 = product_sELL_AVX512<111>;
static auto product112_sELL_AVX512 = product_sELL_AVX512<112>;
static auto product113_sELL_AVX512 = product_sELL_AVX512<113>;
static auto product114_sELL_AVX512 = product_sELL_AVX512<114>;
static auto product115_sELL_AVX512 = product_sELL_AVX512<115>;
static auto product116_sELL_AVX512 = product_sELL_AVX512<116>;
static auto product117_sELL_AVX512 = product_sELL_AVX512<117>;
static auto product118_sELL_AVX512 = product_sELL_AVX512<118>;
static auto product119_sELL_AVX512 = product_sELL_AVX512<119>;
static auto product120_sELL_AVX512 = product_sELL_AVX512<120>;
static auto product121_sELL_AVX512 = product_sELL_AVX512<121>;
static auto product122_sELL_AVX512 = product_sELL_AVX512<122>;
static auto product123_sELL_AVX512 = product_sELL_AVX512<123>;
static auto product124_sELL_AVX512 = product_sELL_AVX512<124>;
static auto product125_sELL_AVX512 = product_sELL_AVX512<125>;
static auto product126_sELL_AVX512 = product_sELL_AVX512<126>;
static auto product127_sELL_AVX512 = product_sELL_AVX512<127>;
static auto product128_sELL_AVX512 = product_sELL_AVX512<128>;


static fcn_k kernelList[512] =
{
product001_sELL_AVX064,
product002_sELL_AVX064,
product003_sELL_AVX064,
product004_sELL_AVX064,
product005_sELL_AVX064,
product006_sELL_AVX064,
product007_sELL_AVX064,
product008_sELL_AVX064,
product009_sELL_AVX064,
product010_sELL_AVX064,
product011_sELL_AVX064,
product012_sELL_AVX064,
product013_sELL_AVX064,
product014_sELL_AVX064,
product015_sELL_AVX064,
product016_sELL_AVX064,
product017_sELL_AVX064,
product018_sELL_AVX064,
product019_sELL_AVX064,
product020_sELL_AVX064,
product021_sELL_AVX064,
product022_sELL_AVX064,
product023_sELL_AVX064,
product024_sELL_AVX064,
product025_sELL_AVX064,
product026_sELL_AVX064,
product027_sELL_AVX064,
product028_sELL_AVX064,
product029_sELL_AVX064,
product030_sELL_AVX064,
product031_sELL_AVX064,
product032_sELL_AVX064,
product033_sELL_AVX064,
product034_sELL_AVX064,
product035_sELL_AVX064,
product036_sELL_AVX064,
product037_sELL_AVX064,
product038_sELL_AVX064,
product039_sELL_AVX064,
product040_sELL_AVX064,
product041_sELL_AVX064,
product042_sELL_AVX064,
product043_sELL_AVX064,
product044_sELL_AVX064,
product045_sELL_AVX064,
product046_sELL_AVX064,
product047_sELL_AVX064,
product048_sELL_AVX064,
product049_sELL_AVX064,
product050_sELL_AVX064,
product051_sELL_AVX064,
product052_sELL_AVX064,
product053_sELL_AVX064,
product054_sELL_AVX064,
product055_sELL_AVX064,
product056_sELL_AVX064,
product057_sELL_AVX064,
product058_sELL_AVX064,
product059_sELL_AVX064,
product060_sELL_AVX064,
product061_sELL_AVX064,
product062_sELL_AVX064,
product063_sELL_AVX064,
product064_sELL_AVX064,
product065_sELL_AVX064,
product066_sELL_AVX064,
product067_sELL_AVX064,
product068_sELL_AVX064,
product069_sELL_AVX064,
product070_sELL_AVX064,
product071_sELL_AVX064,
product072_sELL_AVX064,
product073_sELL_AVX064,
product074_sELL_AVX064,
product075_sELL_AVX064,
product076_sELL_AVX064,
product077_sELL_AVX064,
product078_sELL_AVX064,
product079_sELL_AVX064,
product080_sELL_AVX064,
product081_sELL_AVX064,
product082_sELL_AVX064,
product083_sELL_AVX064,
product084_sELL_AVX064,
product085_sELL_AVX064,
product086_sELL_AVX064,
product087_sELL_AVX064,
product088_sELL_AVX064,
product089_sELL_AVX064,
product090_sELL_AVX064,
product091_sELL_AVX064,
product092_sELL_AVX064,
product093_sELL_AVX064,
product094_sELL_AVX064,
product095_sELL_AVX064,
product096_sELL_AVX064,
product097_sELL_AVX064,
product098_sELL_AVX064,
product099_sELL_AVX064,
product100_sELL_AVX064,
product101_sELL_AVX064,
product102_sELL_AVX064,
product103_sELL_AVX064,
product104_sELL_AVX064,
product105_sELL_AVX064,
product106_sELL_AVX064,
product107_sELL_AVX064,
product108_sELL_AVX064,
product109_sELL_AVX064,
product110_sELL_AVX064,
product111_sELL_AVX064,
product112_sELL_AVX064,
product113_sELL_AVX064,
product114_sELL_AVX064,
product115_sELL_AVX064,
product116_sELL_AVX064,
product117_sELL_AVX064,
product118_sELL_AVX064,
product119_sELL_AVX064,
product120_sELL_AVX064,
product121_sELL_AVX064,
product122_sELL_AVX064,
product123_sELL_AVX064,
product124_sELL_AVX064,
product125_sELL_AVX064,
product126_sELL_AVX064,
product127_sELL_AVX064,
product128_sELL_AVX064,
product001_sELL_AVX128,
product002_sELL_AVX128,
product003_sELL_AVX128,
product004_sELL_AVX128,
product005_sELL_AVX128,
product006_sELL_AVX128,
product007_sELL_AVX128,
product008_sELL_AVX128,
product009_sELL_AVX128,
product010_sELL_AVX128,
product011_sELL_AVX128,
product012_sELL_AVX128,
product013_sELL_AVX128,
product014_sELL_AVX128,
product015_sELL_AVX128,
product016_sELL_AVX128,
product017_sELL_AVX128,
product018_sELL_AVX128,
product019_sELL_AVX128,
product020_sELL_AVX128,
product021_sELL_AVX128,
product022_sELL_AVX128,
product023_sELL_AVX128,
product024_sELL_AVX128,
product025_sELL_AVX128,
product026_sELL_AVX128,
product027_sELL_AVX128,
product028_sELL_AVX128,
product029_sELL_AVX128,
product030_sELL_AVX128,
product031_sELL_AVX128,
product032_sELL_AVX128,
product033_sELL_AVX128,
product034_sELL_AVX128,
product035_sELL_AVX128,
product036_sELL_AVX128,
product037_sELL_AVX128,
product038_sELL_AVX128,
product039_sELL_AVX128,
product040_sELL_AVX128,
product041_sELL_AVX128,
product042_sELL_AVX128,
product043_sELL_AVX128,
product044_sELL_AVX128,
product045_sELL_AVX128,
product046_sELL_AVX128,
product047_sELL_AVX128,
product048_sELL_AVX128,
product049_sELL_AVX128,
product050_sELL_AVX128,
product051_sELL_AVX128,
product052_sELL_AVX128,
product053_sELL_AVX128,
product054_sELL_AVX128,
product055_sELL_AVX128,
product056_sELL_AVX128,
product057_sELL_AVX128,
product058_sELL_AVX128,
product059_sELL_AVX128,
product060_sELL_AVX128,
product061_sELL_AVX128,
product062_sELL_AVX128,
product063_sELL_AVX128,
product064_sELL_AVX128,
product065_sELL_AVX128,
product066_sELL_AVX128,
product067_sELL_AVX128,
product068_sELL_AVX128,
product069_sELL_AVX128,
product070_sELL_AVX128,
product071_sELL_AVX128,
product072_sELL_AVX128,
product073_sELL_AVX128,
product074_sELL_AVX128,
product075_sELL_AVX128,
product076_sELL_AVX128,
product077_sELL_AVX128,
product078_sELL_AVX128,
product079_sELL_AVX128,
product080_sELL_AVX128,
product081_sELL_AVX128,
product082_sELL_AVX128,
product083_sELL_AVX128,
product084_sELL_AVX128,
product085_sELL_AVX128,
product086_sELL_AVX128,
product087_sELL_AVX128,
product088_sELL_AVX128,
product089_sELL_AVX128,
product090_sELL_AVX128,
product091_sELL_AVX128,
product092_sELL_AVX128,
product093_sELL_AVX128,
product094_sELL_AVX128,
product095_sELL_AVX128,
product096_sELL_AVX128,
product097_sELL_AVX128,
product098_sELL_AVX128,
product099_sELL_AVX128,
product100_sELL_AVX128,
product101_sELL_AVX128,
product102_sELL_AVX128,
product103_sELL_AVX128,
product104_sELL_AVX128,
product105_sELL_AVX128,
product106_sELL_AVX128,
product107_sELL_AVX128,
product108_sELL_AVX128,
product109_sELL_AVX128,
product110_sELL_AVX128,
product111_sELL_AVX128,
product112_sELL_AVX128,
product113_sELL_AVX128,
product114_sELL_AVX128,
product115_sELL_AVX128,
product116_sELL_AVX128,
product117_sELL_AVX128,
product118_sELL_AVX128,
product119_sELL_AVX128,
product120_sELL_AVX128,
product121_sELL_AVX128,
product122_sELL_AVX128,
product123_sELL_AVX128,
product124_sELL_AVX128,
product125_sELL_AVX128,
product126_sELL_AVX128,
product127_sELL_AVX128,
product128_sELL_AVX128,
product001_sELL_AVX256,
product002_sELL_AVX256,
product003_sELL_AVX256,
product004_sELL_AVX256,
product005_sELL_AVX256,
product006_sELL_AVX256,
product007_sELL_AVX256,
product008_sELL_AVX256,
product009_sELL_AVX256,
product010_sELL_AVX256,
product011_sELL_AVX256,
product012_sELL_AVX256,
product013_sELL_AVX256,
product014_sELL_AVX256,
product015_sELL_AVX256,
product016_sELL_AVX256,
product017_sELL_AVX256,
product018_sELL_AVX256,
product019_sELL_AVX256,
product020_sELL_AVX256,
product021_sELL_AVX256,
product022_sELL_AVX256,
product023_sELL_AVX256,
product024_sELL_AVX256,
product025_sELL_AVX256,
product026_sELL_AVX256,
product027_sELL_AVX256,
product028_sELL_AVX256,
product029_sELL_AVX256,
product030_sELL_AVX256,
product031_sELL_AVX256,
product032_sELL_AVX256,
product033_sELL_AVX256,
product034_sELL_AVX256,
product035_sELL_AVX256,
product036_sELL_AVX256,
product037_sELL_AVX256,
product038_sELL_AVX256,
product039_sELL_AVX256,
product040_sELL_AVX256,
product041_sELL_AVX256,
product042_sELL_AVX256,
product043_sELL_AVX256,
product044_sELL_AVX256,
product045_sELL_AVX256,
product046_sELL_AVX256,
product047_sELL_AVX256,
product048_sELL_AVX256,
product049_sELL_AVX256,
product050_sELL_AVX256,
product051_sELL_AVX256,
product052_sELL_AVX256,
product053_sELL_AVX256,
product054_sELL_AVX256,
product055_sELL_AVX256,
product056_sELL_AVX256,
product057_sELL_AVX256,
product058_sELL_AVX256,
product059_sELL_AVX256,
product060_sELL_AVX256,
product061_sELL_AVX256,
product062_sELL_AVX256,
product063_sELL_AVX256,
product064_sELL_AVX256,
product065_sELL_AVX256,
product066_sELL_AVX256,
product067_sELL_AVX256,
product068_sELL_AVX256,
product069_sELL_AVX256,
product070_sELL_AVX256,
product071_sELL_AVX256,
product072_sELL_AVX256,
product073_sELL_AVX256,
product074_sELL_AVX256,
product075_sELL_AVX256,
product076_sELL_AVX256,
product077_sELL_AVX256,
product078_sELL_AVX256,
product079_sELL_AVX256,
product080_sELL_AVX256,
product081_sELL_AVX256,
product082_sELL_AVX256,
product083_sELL_AVX256,
product084_sELL_AVX256,
product085_sELL_AVX256,
product086_sELL_AVX256,
product087_sELL_AVX256,
product088_sELL_AVX256,
product089_sELL_AVX256,
product090_sELL_AVX256,
product091_sELL_AVX256,
product092_sELL_AVX256,
product093_sELL_AVX256,
product094_sELL_AVX256,
product095_sELL_AVX256,
product096_sELL_AVX256,
product097_sELL_AVX256,
product098_sELL_AVX256,
product099_sELL_AVX256,
product100_sELL_AVX256,
product101_sELL_AVX256,
product102_sELL_AVX256,
product103_sELL_AVX256,
product104_sELL_AVX256,
product105_sELL_AVX256,
product106_sELL_AVX256,
product107_sELL_AVX256,
product108_sELL_AVX256,
product109_sELL_AVX256,
product110_sELL_AVX256,
product111_sELL_AVX256,
product112_sELL_AVX256,
product113_sELL_AVX256,
product114_sELL_AVX256,
product115_sELL_AVX256,
product116_sELL_AVX256,
product117_sELL_AVX256,
product118_sELL_AVX256,
product119_sELL_AVX256,
product120_sELL_AVX256,
product121_sELL_AVX256,
product122_sELL_AVX256,
product123_sELL_AVX256,
product124_sELL_AVX256,
product125_sELL_AVX256,
product126_sELL_AVX256,
product127_sELL_AVX256,
product128_sELL_AVX256,
product001_sELL_AVX512,
product002_sELL_AVX512,
product003_sELL_AVX512,
product004_sELL_AVX512,
product005_sELL_AVX512,
product006_sELL_AVX512,
product007_sELL_AVX512,
product008_sELL_AVX512,
product009_sELL_AVX512,
product010_sELL_AVX512,
product011_sELL_AVX512,
product012_sELL_AVX512,
product013_sELL_AVX512,
product014_sELL_AVX512,
product015_sELL_AVX512,
product016_sELL_AVX512,
product017_sELL_AVX512,
product018_sELL_AVX512,
product019_sELL_AVX512,
product020_sELL_AVX512,
product021_sELL_AVX512,
product022_sELL_AVX512,
product023_sELL_AVX512,
product024_sELL_AVX512,
product025_sELL_AVX512,
product026_sELL_AVX512,
product027_sELL_AVX512,
product028_sELL_AVX512,
product029_sELL_AVX512,
product030_sELL_AVX512,
product031_sELL_AVX512,
product032_sELL_AVX512,
product033_sELL_AVX512,
product034_sELL_AVX512,
product035_sELL_AVX512,
product036_sELL_AVX512,
product037_sELL_AVX512,
product038_sELL_AVX512,
product039_sELL_AVX512,
product040_sELL_AVX512,
product041_sELL_AVX512,
product042_sELL_AVX512,
product043_sELL_AVX512,
product044_sELL_AVX512,
product045_sELL_AVX512,
product046_sELL_AVX512,
product047_sELL_AVX512,
product048_sELL_AVX512,
product049_sELL_AVX512,
product050_sELL_AVX512,
product051_sELL_AVX512,
product052_sELL_AVX512,
product053_sELL_AVX512,
product054_sELL_AVX512,
product055_sELL_AVX512,
product056_sELL_AVX512,
product057_sELL_AVX512,
product058_sELL_AVX512,
product059_sELL_AVX512,
product060_sELL_AVX512,
product061_sELL_AVX512,
product062_sELL_AVX512,
product063_sELL_AVX512,
product064_sELL_AVX512,
product065_sELL_AVX512,
product066_sELL_AVX512,
product067_sELL_AVX512,
product068_sELL_AVX512,
product069_sELL_AVX512,
product070_sELL_AVX512,
product071_sELL_AVX512,
product072_sELL_AVX512,
product073_sELL_AVX512,
product074_sELL_AVX512,
product075_sELL_AVX512,
product076_sELL_AVX512,
product077_sELL_AVX512,
product078_sELL_AVX512,
product079_sELL_AVX512,
product080_sELL_AVX512,
product081_sELL_AVX512,
product082_sELL_AVX512,
product083_sELL_AVX512,
product084_sELL_AVX512,
product085_sELL_AVX512,
product086_sELL_AVX512,
product087_sELL_AVX512,
product088_sELL_AVX512,
product089_sELL_AVX512,
product090_sELL_AVX512,
product091_sELL_AVX512,
product092_sELL_AVX512,
product093_sELL_AVX512,
product094_sELL_AVX512,
product095_sELL_AVX512,
product096_sELL_AVX512,
product097_sELL_AVX512,
product098_sELL_AVX512,
product099_sELL_AVX512,
product100_sELL_AVX512,
product101_sELL_AVX512,
product102_sELL_AVX512,
product103_sELL_AVX512,
product104_sELL_AVX512,
product105_sELL_AVX512,
product106_sELL_AVX512,
product107_sELL_AVX512,
product108_sELL_AVX512,
product109_sELL_AVX512,
product110_sELL_AVX512,
product111_sELL_AVX512,
product112_sELL_AVX512,
product113_sELL_AVX512,
product114_sELL_AVX512,
product115_sELL_AVX512,
product116_sELL_AVX512,
product117_sELL_AVX512,
product118_sELL_AVX512,
product119_sELL_AVX512,
product120_sELL_AVX512,
product121_sELL_AVX512,
product122_sELL_AVX512,
product123_sELL_AVX512,
product124_sELL_AVX512,
product125_sELL_AVX512,
product126_sELL_AVX512,
product127_sELL_AVX512,
product128_sELL_AVX512
};


void
matVec_BlockRowPerm_sELL_CPU
(
  idx_k                                      nBlocks,
  idx_k          const * const  __restrict__ rlo,
  idx_k          const * const  __restrict__ rhi,
  fcn_k          const * const  __restrict__ productKernels,
  idx_k          const * const  __restrict__ n, //=Permutation.pNeigh();
  LDUoffDiag_k   const * const  __restrict__ u, //=Permutation.upper( randomMatrix.upper() );
  LDUoffDiag_k   const * const  __restrict__ l, //=Permutation.lower( randomMatrix.lower() );
  fld_k          const * const  __restrict__ x, //=Permutation.x(     randomMatrix.x()     );
  fld_k                * const  __restrict__ b  //=m_result;
)
{

  /* Call the kernels */
  idx_k cnt = 0;
  for ( unsigned int i=0; i<nBlocks; ++i )
  {
    // printf( "Kernel Call %u, (%p)\n", i, productKernels[i] );
    // Call the kernel
    (productKernels[i])( &cnt, rlo[i], rhi[i], u, l, n, x, b );
  };
  /* Exit point */
  return;

};
#endif











#ifdef __TEST_NATIVE_CUDA__
__device__ double MYatomicAdd_sELL(fld_k* address, fld_k val)
{
    unsigned long long int* address_as_ull = (unsigned long long int*)address;
    unsigned long long int old = *address_as_ull, assumed;
    do {
        assumed = old;
        old = atomicCAS(address_as_ull, assumed,
                __double_as_longlong(val + __longlong_as_double(assumed)));
    } while (assumed != old);
    return __longlong_as_double(old);
}

__global__
void
matVec_BlockRowPerm_sELL_nativeCUDA
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Compute the index of hte face
  int iFace = blockIdx.x*blockDim.x + threadIdx.x;
  // relax the linear system
  //if ( iFace < nInternalFaces )
  if ( iFace < nInternalFaces )
  {
    // Indirect access indices
    idx_k iOwn = owner[iFace];
    idx_k iNei = neigh[iFace];
    // compute the increments
    fld_k tmp1 = upper[iFace]*loc_psi[iNei];
    fld_k tmp2 = lower[iFace]*loc_psi[iOwn];
    // Accumulate the multiplication
    MYatomicAdd_sELL( &(Apsi[iOwn]), tmp1 );
    MYatomicAdd_sELL( &(Apsi[iNei]), tmp2 );
  };
  // Exit point
  return;
};
#endif


#ifdef __TEST_OPEN_ACC__
void
matVec_BlockRowPerm_sELL_openACC
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // relax the linear system
  #pragma acc parallel loop deviceptr(owner,neigh,upper,lower,loc_psi,Apsi)
  for ( idx_k iFace=0; iFace<nInternalFaces; iFace++ )
  {
    // Indirect access indices
    idx_k iOwn = owner[iFace];
    idx_k iNei = neigh[iFace];
    #pragma acc atomic update
    Apsi[iOwn] += upper[iFace]*loc_psi[iNei];

    #pragma acc atomic update
    Apsi[iNei] += lower[iFace]*loc_psi[iOwn];
  };
  // Exit point
  return;
};
#endif

#ifdef __TEST_OPEN_MP__
void
matVec_BlockRowPerm_sELL_openMP
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  idx_k iFace;
  idx_k iOwn;
  idx_k iNei;
  idx_k iPrev;
  idx_k iNext;
  // relax the linear system
  for ( iFace=0; iFace<nInternalFaces; iFace++ )
  {
    iOwn = owner[iFace];
    iNei = neigh[iFace];
    iPrev = iComp*cellsStride+iOwn;
    iNext = iComp*cellsStride+iNei;
    Apsi[iOwn] += upper[iFace]*psi[iNext];
    Apsi[iNei] += lower[iFace]*psi[iPrev];
  };
  // Exit point
  return;
};
#endif

#endif
