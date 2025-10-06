// TAKEN FROM https://stackoverflow.com/questions/2637700/is-it-possible-to-roll-a-significantly-faster-version-of-sqrt
// LUT for fast sqrt of floats. Table will be consist of 2 parts, half for sqrt(X) and half for sqrt(2X).
const int nBitsForSQRTprecision = 11;                       // Use only 11 most sagnificant bits from the 23 of float. We can use 15 bits instead. It will produce less error but take more place in a memory. 
const int nUnusedBits   = 23 - nBitsForSQRTprecision;       // Amount of bits we will disregard
const int tableSize     = (1 << (nBitsForSQRTprecision+1)); // 2^nBits*2 because we have 2 halves of the table.
static short sqrtTab[tableSize]; 
static unsigned char is_sqrttab_initialized = FALSE;        // Once initialized will be true

// Table of precalculated sqrt() for future fast calculation. Approximates the exact with an error of about 0.5%
// Note: To access the bits of a float in C quickly we must misuse pointers.
// More info in: http://en.wikipedia.org/wiki/Single_precision
void build_fsqrt_table(void){
    unsigned short i;
    float f;
    UINT32 *fi = (UINT32*)&f;

    if (is_sqrttab_initialized)
        return;

    const int halfTableSize = (tableSize>>1);
    for (i=0; i < halfTableSize; i++){
         *fi = 0;
         *fi = (i << nUnusedBits) | (127 << 23); // Build a float with the bit pattern i as mantissa, and an exponent of 0, stored as 127

         // Take the square root then strip the first 'nBitsForSQRTprecision' bits of the mantissa into the table
         f = sqrtf(f);
         sqrtTab[i] = (short)((*fi & 0x7fffff) >> nUnusedBits);

         // Repeat the process, this time with an exponent of 1, stored as 128
         *fi = 0;
         *fi = (i << nUnusedBits) | (128 << 23);
         f = sqrtf(f);
         sqrtTab[i+halfTableSize] = (short)((*fi & 0x7fffff) >> nUnusedBits);
    }
    is_sqrttab_initialized = TRUE;
}

// Calculation of a square root. Divide the exponent of float by 2 and sqrt() its mantissa using the precalculated table.
float fast_float_sqrt(float n){
    if (n <= 0.f) 
        return 0.f;                           // On 0 or negative return 0.
    UINT32 *num = (UINT32*)&n;
    short e;                                  // Exponent
    e = (*num >> 23) - 127;                   // In 'float' the exponent is stored with 127 added.
    *num &= 0x7fffff;                         // leave only the mantissa 

    // If the exponent is odd so we have to look it up in the second half of the lookup table, so we set the high bit.
    const int halfTableSize = (tableSize>>1);
    const int secondHalphTableIdBit = halfTableSize << nUnusedBits;
    if (e & 0x01) 
        *num |= secondHalphTableIdBit;  
    e >>= 1;                                  // Divide the exponent by two (note that in C the shift operators are sign preserving for signed operands

    // Do the table lookup, based on the quaternary mantissa, then reconstruct the result back into a float
    *num = ((sqrtTab[*num >> nUnusedBits]) << nUnusedBits) | ((e + 127) << 23);
    return n;
}