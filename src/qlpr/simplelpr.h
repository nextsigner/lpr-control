#ifndef SIMPLELPR_H
#define SIMPLELPR_H

/*
    file SimpleLPR.h

    C interface for SimpleLPR

    This headers exposes a pure C API for the classes defined in SimpleLPR.hpp

    version: 2.1.0.1
    date: 11/11/2009

    (c) Copyright Warelogic, 2009
    All rights reserved. Copying or other reproduction of this
    program except for archival purposes is prohibited without
    written consent of Warelogic.
*/

#if !defined(AFX_SIMPLELPR_H__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_)
#define AFX_SIMPLELPR_H__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// The following ifdef block is the standard way of creating macros that make exporting
// from a DLL simpler. All files within this DLL are compiled with the SIMPLELPR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SIMPLELPR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#if defined(SIMPLELPR_EXPORTS)
#	if !defined(SIMPLELPR_APICLASS)
#		define SIMPLELPR_APICLASS	__declspec(dllexport)
#	endif
#	if !defined(SIMPLELPR_API)
#		define SIMPLELPR_API		__declspec(dllexport)
#	endif
#	if !defined(EXPIMP_TEMPLATE)
#		define EXPIMP_TEMPLATE
#	endif
#else
#	if !defined(SIMPLELPR_APICLASS)
#		define SIMPLELPR_APICLASS	__declspec(dllimport)
#	endif
#	if !defined(SIMPLELPR_API)
#		define SIMPLELPR_API		__declspec(dllimport)
#	endif
#	if !defined(EXPIMP_TEMPLATE)
#		define EXPIMP_TEMPLATE		extern
#	endif
#endif

#if !defined(_WIN64)
#	define	SIMPLELPR_CALL	_stdcall
#else
#	define	SIMPLELPR_CALL
#endif

#ifdef	__cplusplus
extern "C" {
#endif

// Some base types
typedef int SIMPLELPR_BOOL;
typedef __int32 SIMPLELPR_HRESULT;
typedef unsigned __int32 SIMPLELPR_SIZE_T;
typedef float SIMPLELPR_REAL_T;

typedef void* SIMPLELPR_Handle;

#pragma pack(push,8)	// Set structure packing alignment

// SIMPLELPR_VersionNumber
// SimpleLPR version number.
struct SIMPLELPR_VersionNumber
{
    unsigned __int32 A;	 // First element of the of SimpleLPR_Native.dll product number
    unsigned __int32 B;	 // Second element of the of SimpleLPR_Native.dll product number
    unsigned __int32 C;	 // Third element of the of SimpleLPR_Native.dll product number
    unsigned __int32 D;	 // Fourth element of the of SimpleLPR_Native.dll product number
};

// SIMPLELPR_Rect
// This class encapsulates a rectangle.
struct SIMPLELPR_Rect
{
    __int32 left;	// The leftmost coordinate
    __int32 top;	// The topmost coordinate
    __int32 width;  // The rectangle width
    __int32 height; // The rectangle height
};

// SIMPLELPR_Element
// Encapsulates a candidate character in a license plate.
struct SIMPLELPR_Element
{
    SIMPLELPR_REAL_T fConfidence; // 'Goodness' of the recognition. Its range is 0 to 1 and can be used to rank candidates although
                                  // it should not be regarded as a probability. In general, a 2x goodness value is not twice as good as x.
    struct SIMPLELPR_Rect boundingBox;   // Bounding box of the character, in pixel coordinates.
    wchar_t glyph;	    // Unicode representation of the character.
};

#pragma pack( pop )

// The routines below are only wrappers of the interfaces defined in SimpleLPR.hpp.
// For information on those functions see the comments in SimpleLPR.hpp.

// See IReferenceCounted.
SIMPLELPR_API void SIMPLELPR_CALL SIMPLELPR_ReferenceCounted_addRef( SIMPLELPR_Handle hObj );
SIMPLELPR_API void SIMPLELPR_CALL SIMPLELPR_ReferenceCounted_release( SIMPLELPR_Handle hObj );

// See IErrorInfo.
SIMPLELPR_API SIMPLELPR_HRESULT SIMPLELPR_CALL SIMPLELPR_ErrorInfo_errorCode_get( SIMPLELPR_Handle hErrInfo );
SIMPLELPR_API const wchar_t * SIMPLELPR_CALL SIMPLELPR_ErrorInfo_description_get( SIMPLELPR_Handle hErrInfo );

// See ICandidate.
SIMPLELPR_API const wchar_t *SIMPLELPR_CALL SIMPLELPR_Candidate_text_get( SIMPLELPR_Handle hCandidate );
SIMPLELPR_API const wchar_t *SIMPLELPR_CALL SIMPLELPR_Candidate_country_get( SIMPLELPR_Handle hCandidate );
SIMPLELPR_API SIMPLELPR_REAL_T SIMPLELPR_CALL SIMPLELPR_Candidate_confidence_get( SIMPLELPR_Handle hCandidate );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_Candidate_brightBackground_get( SIMPLELPR_Handle hCandidate );
SIMPLELPR_API SIMPLELPR_SIZE_T SIMPLELPR_CALL SIMPLELPR_Candidate_numElements_get( SIMPLELPR_Handle hCandidate );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_Candidate_element_get( SIMPLELPR_Handle hCandidate, SIMPLELPR_SIZE_T id, /*[out]*/struct SIMPLELPR_Element *pElem );

// See ICandidates.
SIMPLELPR_API SIMPLELPR_SIZE_T SIMPLELPR_CALL SIMPLELPR_Candidates_numCandidates_get( SIMPLELPR_Handle hCandidates );
SIMPLELPR_API SIMPLELPR_Handle /*hCandidate*/ SIMPLELPR_CALL SIMPLELPR_Candidates_candidate_get( SIMPLELPR_Handle hCandidates, SIMPLELPR_SIZE_T id );

// See IProcessor.
SIMPLELPR_API SIMPLELPR_Handle /*hCandidates*/ SIMPLELPR_CALL SIMPLELPR_Processor_analyzeBuffer( SIMPLELPR_Handle hProcessor,
                                                                                                 const void *pcvImgData,
                                                                                                 SIMPLELPR_SIZE_T widthStep,
                                                                                                 SIMPLELPR_SIZE_T width,
                                                                                                 SIMPLELPR_SIZE_T height,
                                                                                                 SIMPLELPR_SIZE_T maxCharHeight );

SIMPLELPR_API SIMPLELPR_Handle /*hCandidates*/ SIMPLELPR_CALL SIMPLELPR_Processor_analyzeBuffer_C3( SIMPLELPR_Handle hProcessor,
                                                                                                    const void *pcvImgData,
                                                                                                    SIMPLELPR_SIZE_T widthStep,
                                                                                                    SIMPLELPR_SIZE_T width,
                                                                                                    SIMPLELPR_SIZE_T height,
                                                                                                    SIMPLELPR_SIZE_T maxCharHeight,
                                                                                                    SIMPLELPR_REAL_T fWeight0,
                                                                                                    SIMPLELPR_REAL_T fWeight1,
                                                                                                    SIMPLELPR_REAL_T fWeight2 );

SIMPLELPR_API SIMPLELPR_Handle /*hCandidates*/ SIMPLELPR_CALL SIMPLELPR_Processor_analyzeBuffer_C4( SIMPLELPR_Handle hProcessor,
                                                                                                    const void *pcvImgData,
                                                                                                    SIMPLELPR_SIZE_T widthStep,
                                                                                                    SIMPLELPR_SIZE_T width,
                                                                                                    SIMPLELPR_SIZE_T height,
                                                                                                    SIMPLELPR_SIZE_T maxCharHeight,
                                                                                                    SIMPLELPR_REAL_T fWeight0,
                                                                                                    SIMPLELPR_REAL_T fWeight1,
                                                                                                    SIMPLELPR_REAL_T fWeight2 );

SIMPLELPR_API SIMPLELPR_Handle /*hCandidates*/ SIMPLELPR_CALL SIMPLELPR_Processor_analyzeFile( SIMPLELPR_Handle hProcessor,
                                                                                               const wchar_t *pcwsImgPath,
                                                                                               SIMPLELPR_SIZE_T maxCharHeight );

// See ISimpleLPR.

SIMPLELPR_API SIMPLELPR_SIZE_T SIMPLELPR_CALL SIMPLELPR_numSupportedCountries_get( SIMPLELPR_Handle hEngine );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_countryCode_get( SIMPLELPR_Handle hEngine, SIMPLELPR_SIZE_T id, /*[out]*/const wchar_t **ppcwsCode );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_countryFromIdWeight_get( SIMPLELPR_Handle hEngine, SIMPLELPR_SIZE_T id, /*[out]*/SIMPLELPR_REAL_T *pfWeight );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_countryFromStringWeight_get( SIMPLELPR_Handle hEngine, const wchar_t *id, /*[out]*/SIMPLELPR_REAL_T *pfWeight );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_countryFromIdWeight_set( SIMPLELPR_Handle hEngine, SIMPLELPR_SIZE_T id, SIMPLELPR_REAL_T fWeight );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_countryFromStringWeight_set( SIMPLELPR_Handle hEngine, const wchar_t *id, SIMPLELPR_REAL_T fWeight );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_realizeCountryWeights(SIMPLELPR_Handle hEngine );
SIMPLELPR_API SIMPLELPR_Handle /*hErrorInfo*/ SIMPLELPR_CALL SIMPLELPR_lastError_get( SIMPLELPR_Handle hEngine, SIMPLELPR_BOOL bClear );
SIMPLELPR_API SIMPLELPR_Handle /*hProcessor*/ SIMPLELPR_CALL SIMPLELPR_createProcessor( SIMPLELPR_Handle hEngine );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_productKeyFromFile_set( SIMPLELPR_Handle hEngine, const wchar_t *productKeyPath );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_productKeyFromBuffer_set( SIMPLELPR_Handle hEngine, const void *key, SIMPLELPR_SIZE_T keySize );
SIMPLELPR_API SIMPLELPR_BOOL SIMPLELPR_CALL SIMPLELPR_versionNumber_get( SIMPLELPR_Handle hEngine, /*[out]*/struct SIMPLELPR_VersionNumber *pVersion );

// See Setup function in SimpleLPR.hpp
SIMPLELPR_API SIMPLELPR_Handle SIMPLELPR_CALL SIMPLELPR_Setup( void );

#ifdef	__cplusplus
} // extern "C"
#endif

#endif // !defined(AFX_SIMPLELPR_H__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_)

#endif // SIMPLELPR_H

