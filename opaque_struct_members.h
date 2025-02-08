#ifdef USE_BIT_MASK_METHOD 

  #ifdef USE_PRIVATE_SCOPE
    #define PRIVATE(type, var) type var;
  #else
    #define PRIVATE(type, var) long: 8*sizeof(type);
  #endif

#else

  #ifdef USE_PRIVATE_SCOPE
    #define PRIVATE(STRUCT_PRIVATE_MEMBERS) long: 64; struct { STRUCT_PRIVATE_MEMBERS };
  #else
    #define PRIVATE(STRUCT_PRIVATE_MEMBERS) long: 64; char _[sizeof(struct { /*long: 64;*/ STRUCT_PRIVATE_MEMBERS }) /*- 8*/];
  #endif

#endif
