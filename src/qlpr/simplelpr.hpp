#ifndef SIMPLELPR_HPP
#define SIMPLELPR_HPP

/*
    file SimpleLPR.hpp

    C++ interface for SimpleLPR
    version: 2.1.0.1
    date: 11/11/2009

    (c) Copyright Warelogic, 2009
    All rights reserved. Copying or other reproduction of this
    program except for archival purposes is prohibited without
    written consent of Warelogic.
*/

#if !defined(AFX_SIMPLELPR_HPP__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_)
#define AFX_SIMPLELPR_HPP__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "simplelpr.h"

namespace SimpleLPR2_Native
{

typedef SIMPLELPR_HRESULT _HRESULT;
typedef SIMPLELPR_SIZE_T _SIZE_T;
typedef SIMPLELPR_REAL_T _REAL_T;

typedef SIMPLELPR_Rect Rect;
typedef SIMPLELPR_Element Element;

// IReferenceCounted
// Base for all interfaces. Manages object life cycle by means of reference counting.
// All factory methods in this library return pointers to objects with their reference count set to 1 or more.
// It is advisable to call the addRef method every time a pointer alias is created.
// Before a pointer goes out of scope the release method should be called. When object reference count
// goes to zero the object is destroyed.
struct IReferenceCounted
{
    // addRef
    // Description: Increments the object reference count by one.
    virtual void addRef( void ) throw () = 0;

    // release
    // Description: Decrements the object reference count by one. When it reaches 0 the object is destroyed.
    virtual void release( void ) throw () = 0;
};

// IErrorInfo
// Error description class. Every time an error occurs an IErrorInfo object is created and kept in thread local storage (TLS).
// To retrieve it use the ISimpleLPR::lastError_get method.
struct IErrorInfo : public IReferenceCounted
{
    // errorCode_get
    // Description: Returns the HRESULT COM like error code.
    virtual _HRESULT errorCode_get() const throw () = 0;
    __declspec(property(get=errorCode_get)) _HRESULT errorCode;

    // description_get
    // Description: Returns the error textual description.
    virtual const wchar_t * description_get() const throw () = 0;
    __declspec(property(get=description_get)) const wchar_t *description;
};

// ICandidate
// Encapsulates a license plate candidate.
struct ICandidate : public IReferenceCounted
{
    // text_get
    // Description: Unicode representation of the license plate string. Separators are represented as white space.
    virtual const wchar_t *text_get() const throw () = 0;
    __declspec(property(get=text_get)) const wchar_t *text;

    // country_get
    // Description: country code string.
    virtual const wchar_t *country_get() const throw () = 0;
    __declspec(property(get=country_get)) const wchar_t *country;

    // confidence_get
    // Description: Returns the overall 'goodness' of the recognition.
    // Currently it is calculated as the minimum goodness value of all individual characters in the license plate.
    virtual _REAL_T confidence_get() const throw () = 0;
    __declspec(property(get=confidence_get)) _REAL_T confidence;

    // brightBackground_get
    // Description: True if the license plate features dark text on a light background. False if otherwise.
    virtual bool brightBackground_get() const throw () = 0;
    __declspec(property(get=brightBackground_get)) bool brightBackground;

    // numElements_get
    // Description: Number of components in the license plate.
    virtual _SIZE_T numElements_get() const throw () = 0;
    __declspec(property(get=numElements_get)) _SIZE_T numElements;

    // element_get
    // Description: Information about the individual chars that make up the license plate. They are listed in the same order
    // as they appear in the text string. To know the physical layout use the Element::bbox field.
    virtual bool element_get( _SIZE_T id, /*[out]*/Element &rElem ) const throw () = 0;
};

// ICandidates
// Encapsulates a collection of license plate candidates.
struct ICandidates : public IReferenceCounted
{
    // numCandidates_get
    // Description: Number of elements in the collection
    virtual _SIZE_T numCandidates_get() const throw () = 0;
    __declspec(property(get=numCandidates_get)) _SIZE_T numCandidates;

    // candidate_get
    // Description: Returns a candidate object given its index.
    // Parameters:
    //	Input.
    //		id : Candidate identifier. Must be <= numCandidates_get()
    // Returns a pointer to the selected candidate object.
    // Remarks: The returned ICandidate object must be deallocated by calling its release method.
    virtual ICandidate *candidate_get( _SIZE_T id ) const throw () = 0;
    __declspec(property(get=candidate_get)) ICandidate *candidate[];
};

// IProcessor
// Provides access to the license plate recognition functionality of SimpleLPR.
// Remarks: This class is not multi-threaded and, therefore, each thread should use a different IProcessor instance.
struct IProcessor : public IReferenceCounted
{
    // analyze
    // Description: Looks for license plate candidates in a memory buffer containing a 8 bit gray scale image.
    // Parameters:
    //	Input.
    //		pImgData: Pointer to the first image row. The image must be 8 bit gray scale and top down:
    //				  the top row of the image is the first row in memory, followed by the next row down.
    //		widthStep: Distance in bytes between starts of consecutive rows in the source image.
    //	    width: Image width in pixels.
    //		height: Image height in pixels.
    //		maxCharHeight: Maximum height in pixels of the characters in the license plate.
    // Returns: A pointer to a ICandidates collection containing all license plate candidates
    //			If something goes wrong returns a NULL pointer, use ISimpleLPR::lastError_get to get the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			The returned ICandidates object must be deallocated by calling its release method.
    virtual ICandidates *analyze( const void *pcvImgData,
                                  _SIZE_T widthStep,
                                  _SIZE_T width,
                                  _SIZE_T height,
                                  _SIZE_T maxCharHeight ) throw () = 0;

    // analyze_C3
    // Description: Looks for license plate candidates in a memory buffer containing a 3-channel 8 bit/channel color image
    //				stored in pixel order. Every pixel in the source image is internally converted to gray scale according to
    //				the following formula:
    //
    //				  L = fWeight0 * C0 + fWeight1 * C1 + fWeight2 * C2
    //
    //				For instance, to convert a RGB image to gray scale according the standard for the NTSC CRT the value
    //				of the weights should be:
    //
    //					Red:   fWeight0 = 0.299
    //					Green: fWeight1 = 0.587
    //					Blue:  fWeight2 = 0.114
    //
    // Parameters:
    //	Input.
    //		pImgData: Pointer to the first image row. The image must be a 3-channel 8 bit/channel color image stored in pixel order
    //                and top down: the top row of the image is the first row in memory, followed by the next row down.
    //		widthStep: Distance in bytes between starts of consecutive rows in the source image.
    //	    width: Image width in pixels.
    //		height: Image height in pixels.
    //		fWeight0: Weight of the first channel.
    //		fWeight1: Weight of the second channel.
    //		fWeight2: Weight of the third channel.
    // Returns: A pointer to a ICandidates collection containing all license plate candidates
    //			If something goes wrong returns a NULL pointer, use ISimpleLPR::lastError_get to get the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			The returned ICandidates object must be deallocated by calling its release method.
    virtual ICandidates *analyze_C3( const void *pcvImgData,
                                     _SIZE_T widthStep,
                                     _SIZE_T width,
                                     _SIZE_T height,
                                     _SIZE_T maxCharHeight,
                                     _REAL_T fWeight0,
                                     _REAL_T fWeight1,
                                     _REAL_T fWeight2 ) throw () = 0;

    // analyze_C4
    // Description: Looks for license plate candidates in a memory buffer containing a 4-channel 8 bit/channel color image
    //				stored in pixel order. The fourth byte in each pixel is not used. Every pixel in the source image is internally
    //				converted to gray scale according to the following formula:
    //
    //				  L = fWeight0 * C0 + fWeight1 * C1 + fWeight2 * C2
    //
    //				For instance, to convert a RGB image to gray scale according the standard for the NTSC CRT the value
    //				of the weights should be:
    //
    //					Red:   fWeight0 = 0.299
    //					Green: fWeight1 = 0.587
    //					Blue:  fWeight2 = 0.114
    //
    // Parameters:
    //	Input.
    //		pImgData: Pointer to the first image row. The image must be a 4-channel 8 bit/channel color image stored in pixel order
    //                and top down: the top row of the image is the first row in memory, followed by the next row down.
    //		widthStep: Distance in bytes between starts of consecutive rows in the source image.
    //	    width: Image width in pixels.
    //		height: Image height in pixels.
    //		fWeight0: Weight of the first channel.
    //		fWeight1: Weight of the second channel.
    //		fWeight2: Weight of the third channel.
    // Returns: A pointer to a ICandidates collection containing all license plate candidates
    //			If something goes wrong returns a NULL pointer, use ISimpleLPR::lastError_get to get the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			The returned ICandidates object must be deallocated by calling its release method.
    virtual ICandidates *analyze_C4( const void *pcvImgData,
                                     _SIZE_T widthStep,
                                     _SIZE_T width,
                                     _SIZE_T height,
                                     _SIZE_T maxCharHeight,
                                     _REAL_T fWeight0,
                                     _REAL_T fWeight1,
                                     _REAL_T fWeight2 ) throw () = 0;

    // analyze
    // Description: Looks for license plate candidates in an image loaded from a .jpg, .png or .tif file.
    //			    The images can be either 24 bit RGB or 8 bit gray scale.
    // Parameters:
    //	Input.
    //		imgPath: Path to a .jpg, .png or .tif file containing a 24 bit RGB or 8 bit gray scale image.
    //		maxCharHeight: Maximum height in pixels of the characters in the license plate.
    // Returns: A pointer to a ICandidates collection containing all license plate candidates
    //			If something goes wrong returns a NULL pointer, use ISimpleLPR::lastError_get to get the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			The returned ICandidates object must be deallocated by calling its release method.
    virtual ICandidates *analyze( const wchar_t * pcwsImgPath,
                                  _SIZE_T maxCharHeight ) throw () = 0;
};

// ISimpleLPR
// Encapsulates the SimpleLPR engine.
struct ISimpleLPR : public IReferenceCounted
{
    // numSupportedCountries_get
    // Description: Returns the number of supported countries.
    virtual _SIZE_T numSupportedCountries_get() const throw () = 0;
    __declspec(property(get=numSupportedCountries_get)) _SIZE_T numSupportedCountries;

    // countryCode_get
    // Description: Given a country index it returns its string identifier.
    // Parameters:
    //	Input.
    //		id: The country index. 0 <= id <= numSupportedCountries_get() - 1
    //  Output.
    //      rpcwsCode: The country unicode string identifier.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool countryCode_get( _SIZE_T id, /*[out]*/const wchar_t *&rpcwsCode ) const throw () = 0;

    // countryWeight_get
    // Description: Given a country index it gets the country relative weight.
    //				Weights are used to break ties when a candidate can belong to multiple countries.
    // Parameters:
    //	Input.
    //		id: The country index. 0 <= id <= numSupportedCountries_get() - 1
    //  Output.
    //      rfWeight: The relative weight of the country.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool countryWeight_get( _SIZE_T id, /*[out]*/_REAL_T &rfWeight ) const throw () = 0;


    // countryWeight_get
    // Description: Given a country string identifier it gets the country relative weight.
    //				Weights are used to break ties when a candidate can belong to multiple countries.
    // Parameters:
    //	Input.
    //		id: The country string identifier. See countryCode_get().
    //  Output.
    //      rfWeight: The relative weight of the country.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool countryWeight_get( const wchar_t *id, /*[out]*/_REAL_T &rfWeight ) const throw () = 0;

    // countryWeight_set
    // Description: Given a country index it sets the country relative weight.
    //				Weights are used to break ties when a candidate can belong to multiple countries.
    // Parameters:
    //	Input.
    //		id: The country index. 0 <= id <= numSupportedCountries_get() - 1
    //      fWeight: The desired country weight. fWeight >= 0
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			Weight must be >= 0. Use a zero weight to effectively disable a specific country.
    virtual bool countryWeight_set( _SIZE_T id, _REAL_T fWeight ) throw () = 0;

    // countryWeight_set
    // Description: Given a country string identifier it sets the country relative weight.
    //				Weights are used to break ties when a candidate can belong to multiple countries.
    // Parameters:
    //	Input.
    //		id: The country string identifier. See countryCode_get().
    //      fWeight: The desired country weight. fWeight >= 0
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			Weight must be >= 0. Use a zero weight to effectively disable a specific country.
    virtual bool countryWeight_set( const wchar_t *id, _REAL_T fWeight ) throw () = 0;

    // realizeCountryWeights
    // Description: Builds internal country verification lookup tables based on which countries are enabled and their relative weights.
    //				Call it once you have finished configuring country weights.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    // Remarks: This method is not multi-threaded.
    //			Depending on the countries selected this method can be time consuming.
    //			After the execution of this method all existing and new IProcessor instances will start using the new weights.
    //			Avoid calling this method when another thread is executing IProcessor::analyze.
    virtual bool realizeCountryWeights() throw () = 0;

    // lastError_get
    // Description: Returns an IErrorInfo object that describes the latest error occurred.
    // Parameters:
    //	Input.
    //		bClear: If true the error state will be cleared after this call.
    // Returns: A IErrorInfo object that describes the latest error, or NULL if no error has occurred since the the application startup or
    //			the last call to lastError_get with bClear set to true.
    // Remarks: This method IS multi-threaded. In particular each thread maintains a TLS slot with error state so threads can be independent
    //			from each other.
    virtual IErrorInfo *lastError_get( bool bClear = true ) throw () = 0;

    // createProcessor
    // Description: Creates a new IProcessor object.
    // Returns: The newly created IProcessor instance, or NULL if the method failed. Use ISimpleLPR::lastError_get to obtain the
    //          error information.
    // Remarks: This method IS multi-threaded.
    //			For this method to succeed, either the product must be within the evaluation period o a valid product key must be supplied
    //			using productKey_set.
    //			The returned IProcessor object must be deallocated by calling its release method.
    virtual IProcessor *createProcessor() throw () = 0;

    // productKey_set
    // Description: Sets the product key from a license file.
    // Parameters:
    //	Input.
    //		productKeyPath: Path to the product key file.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool productKey_set( const wchar_t *productKeyPath ) throw () = 0;

    // productKey_set
    // Description: Sets the product key from a memory buffer.
    // Parameters:
    //	Input.
    //		key: Pointer to the memory buffer.
    //		keySize: Key size in bytes.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool productKey_set( const void *key, _SIZE_T keySize ) throw () = 0;

    // versionInfo_get
    // Description: Gets the version number of SimpleLPR2_native.dll
    //  Output.
    //      rVersion: The 4 element version number.
    // Returns: true if the method succeeded, false otherwise. In the later case use ISimpleLPR::lastError_get to obtain the
    //          error information.
    virtual bool versionNumber_get( /*[out]*/SIMPLELPR_VersionNumber &rVersion ) throw () = 0;
};

typedef ISimpleLPR * (SIMPLELPR_CALL *SimpleLPRSetupFunc)();

extern "C"
{
    // Setup
    // Description: This is the only function exported by the SimpleLPR2.dll
    //				It works a factory method for ISimpleLPR
    // Returns: A pointer to a ISimpleLPR object. The returned ISimpleLPR object must be deallocated by calling its release method.
    // Remarks: It is advised to load SimpleLPR2.dll dinamically
    // Example:
    //			HMODULE hLib = LoadLibraryEx( _T("SimpleLPR2.dll"), NULL, 0 );
    //			SimpleLPR2_Native::SimpleLPRSetupFunc pfnSetup = (SimpleLPR2_Native::SimpleLPRSetupFunc)::GetProcAddress( hLib, "Setup" );
    //			ISimpleLPR *pLPR = (*pfnSetup)();
    //			...
    //			pLPR->release();
    SIMPLELPR_API ISimpleLPR * SIMPLELPR_CALL Setup() throw ();
}

}

#endif // !defined(AFX_SIMPLELPR_HPP__AD8CC432_6769_726F_6E65_7364CADD1A50__INCLUDED_)

#endif // SIMPLELPR_HPP

