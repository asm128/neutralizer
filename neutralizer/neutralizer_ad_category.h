#include "gpk_enum.h"

#ifndef NEUTRALIZER_AD_CATEGORY_H_239084293847
#define NEUTRALIZER_AD_CATEGORY_H_239084293847

namespace ntl
{
	GDEFINE_ENUM_TYPE(AD_SHOP_CATEGORY, uint8_t);
	GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, tours, 0);
	GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, meals, 1);
	GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shops, 2);
	GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shows, 3);

	::gpk::error_t								htmlBoardGenerate				(const ::gpk::view_const_string & contentFileName, const AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output);
	::gpk::error_t								pageCatalog						(const ::gpk::view_const_string & contentFileName, const ::gpk::view_const_string & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_string & title, const ::gpk::view_const_string & lang, ::gpk::array_pod<char_t> & output);
} // namespace

#endif // NEUTRALIZER_AD_CATEGORY_H_239084293847
