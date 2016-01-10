/*
    Copyright (c) 2014-2015, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#include "font_utils.h"

#include <stdio.h>
#include <string.h>

#include "json_utils.h"


void FontLoadFromJSON(Font *f, const char *imgPath, const char *jsonPath)
{
	yajl_val node = JSONReadFile(jsonPath);
	if (node == NULL)
	{
		fprintf(stderr, "Error parsing font JSON '%s'\n", jsonPath);
		goto bail;
	}

	memset(f, 0, sizeof *f);
	// Load definitions from JSON data
	LoadVec2i(&f->Size, node, "Size");
	LoadInt(&f->Stride, node, "Stride");

	// Padding order is: left/top/right/bottom
	const yajl_val paddingNode = JSONFindNode(node, "Padding");
	f->Padding.Left = (int)YAJL_GET_INTEGER(YAJL_GET_ARRAY(paddingNode)->values[0]);
	f->Padding.Top = (int)YAJL_GET_INTEGER(YAJL_GET_ARRAY(paddingNode)->values[1]);
	f->Padding.Right = (int)YAJL_GET_INTEGER(YAJL_GET_ARRAY(paddingNode)->values[2]);
	f->Padding.Bottom = (int)YAJL_GET_INTEGER(YAJL_GET_ARRAY(paddingNode)->values[3]);

	LoadVec2i(&f->Gap, node, "Gap");
	bool proportional = false;
	LoadBool(&proportional, node, "Proportional");

	FontLoad(f, imgPath, proportional);

bail:
	yajl_tree_free(node);
}
