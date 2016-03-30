/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkLocalMatrixImageFilter.h"
#include "SkReadBuffer.h"
#include "SkSpecialImage.h"
#include "SkString.h"

SkLocalMatrixImageFilter::SkLocalMatrixImageFilter(const SkMatrix& localM,
                                                   sk_sp<SkImageFilter> input)
    : INHERITED(&input, 1, nullptr)
    , fLocalM(localM) {
}

SkFlattenable* SkLocalMatrixImageFilter::CreateProc(SkReadBuffer& buffer) {
    SK_IMAGEFILTER_UNFLATTEN_COMMON(common, 1);
    SkMatrix lm;
    buffer.readMatrix(&lm);
    return SkLocalMatrixImageFilter::Make(lm,
                                          sk_ref_sp<SkImageFilter>(common.getInput(0))).release();
}

void SkLocalMatrixImageFilter::flatten(SkWriteBuffer& buffer) const {
    this->INHERITED::flatten(buffer);
    buffer.writeMatrix(fLocalM);
}

sk_sp<SkSpecialImage> SkLocalMatrixImageFilter::onFilterImage(SkSpecialImage* source,
                                                              const Context& ctx,
                                                              SkIPoint* offset) const {
    Context localCtx(SkMatrix::Concat(ctx.ctm(), fLocalM), ctx.clipBounds(), ctx.cache());
    return this->filterInput(0, source, localCtx, offset);
}

SkIRect SkLocalMatrixImageFilter::onFilterBounds(const SkIRect& src, const SkMatrix& matrix,
                                                 MapDirection direction) const {
    return this->getInput(0)->filterBounds(src, SkMatrix::Concat(matrix, fLocalM), direction);
}

#ifndef SK_IGNORE_TO_STRING
void SkLocalMatrixImageFilter::toString(SkString* str) const {
    str->append("SkLocalMatrixImageFilter: (");
    str->append(")");
}
#endif
