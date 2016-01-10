/*
 Copyright (c) 2013-2016, Cong Xu
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
#include "json_utils.h"

#include <stdio.h>
#include <stdlib.h>


static char *ReadFile(const char *filename);
yajl_val JSONReadFile(const char *filename)
{
	yajl_val node = NULL;
	char errbuf[1024];
	char *buf = ReadFile(filename);
	if (buf == NULL)
	{
		fprintf(stderr, "Error reading JSON file '%s'\n", filename);
		goto bail;
	}
	node = yajl_tree_parse(buf, errbuf, sizeof errbuf);
	if (node == NULL)
	{
		fprintf(stderr, "Error parsing font JSON '%s': %s\n", filename, errbuf);
		goto bail;
	}

bail:
	free(buf);
	return node;
}
// Read a file into a dynamic buffer
static char *ReadFile(const char *filename)
{
	FILE *f = fopen(filename, "r");
	char *buf = NULL;
	if (f == NULL) goto bail;
	if (fseek(f, 0L, SEEK_END) != 0) goto bail;
	const long bufsize = ftell(f);
	if (bufsize == -1) goto bail;
	if (fseek(f, 0L, SEEK_SET) != 0) goto bail;
	buf = malloc(bufsize + 1);
	if (buf == NULL) goto bail;
	const size_t readlen = fread(buf, 1, bufsize, f);
	buf[readlen] = '\0';

bail:
	if (f)
	{
		fclose(f);
	}
	return buf;
}

/*
#include "config.h"
#include "weapon.h"
#include "pic_manager.h"
#include "sys_config.h"


void AddIntPair(json_t *parent, const char *name, int number)
{
	char buf[32];
	sprintf(buf, "%d", number);
	json_insert_pair_into_object(parent, name, json_new_number(buf));
}
void AddBoolPair(json_t *parent, const char *name, int value)
{
	json_insert_pair_into_object(parent, name, json_new_bool(value));
}
void AddStringPair(json_t *parent, const char *name, const char *s)
{
	if (!s)
	{
		json_insert_pair_into_object(parent, name, json_new_string(""));
	}
	else
	{
		json_insert_pair_into_object(
			parent, name, json_new_string(json_escape(s)));
	}
}
void AddColorPair(json_t *parent, const char *name, const color_t c)
{
	char buf[8];
	ColorStr(buf, c);
	AddStringPair(parent, name, buf);
}
*/
bool TryLoadValue(yajl_val *node, const char *name)
{
	if (*node == NULL || !YAJL_IS_OBJECT(*node))
	{
		return false;
	}
	const char *path[] = { name, NULL };
	*node = yajl_tree_get(*node, path, yajl_t_any);
	return *node != NULL;
}
void LoadBool(bool *value, yajl_val node, const char *name)
{
	if (!TryLoadValue(&node, name))
	{
		return;
	}
	*value = YAJL_IS_TRUE(node);
}
void LoadInt(int *value, yajl_val node, const char *name)
{
	if (!TryLoadValue(&node, name) || !YAJL_IS_INTEGER(node))
	{
		return;
	}
	*value = YAJL_GET_INTEGER(node);
}
void LoadDouble(double *value, yajl_val node, const char *name)
{
	if (!TryLoadValue(&node, name) || !YAJL_IS_DOUBLE(node))
	{
		return;
	}
	*value = YAJL_GET_DOUBLE(node);
}
void LoadVec2i(Vec2i *value, yajl_val node, const char *name)
{
	if (!TryLoadValue(&node, name) || !YAJL_IS_ARRAY(node))
	{
		return;
	}
	value->x = YAJL_GET_INTEGER(YAJL_GET_ARRAY(node)->values[0]);
	value->y = YAJL_GET_INTEGER(YAJL_GET_ARRAY(node)->values[1]);
}
/*
void LoadStr(char **value, json_t *node, const char *name)
{
	if (!TryLoadValue(&node, name))
	{
		return;
	}
	*value = json_unescape(node->text);
}
char *GetString(json_t *node, const char *name)
{
	return json_unescape(json_find_first_label(node, name)->child->text);
}
void LoadSoundFromNode(Mix_Chunk **value, json_t *node, const char *name)
{
	if (json_find_first_label(node, name) == NULL)
	{
		return;
	}
	if (!TryLoadValue(&node, name))
	{
		return;
	}
	*value = StrSound(node->text);
}
void LoadPic(
	const Pic **value, json_t *node, const char *name, const char *oldPicName)
{
	if (json_find_first_label(node, name))
	{
		char *tmp = GetString(node, name);
		*value = PicManagerGetPic(&gPicManager, tmp);
		CFREE(tmp);
	}
	if ((*value == NULL || ConfigGetBool(&gConfig, "Graphics.OriginalPics")) &&
		json_find_first_label(node, oldPicName))
	{
		int oldPic;
		LoadInt(&oldPic, node, oldPicName);
		*value = PicManagerGetFromOld(&gPicManager, oldPic);
	}
}
void LoadBulletGuns(CArray *guns, json_t *node, const char *name)
{
	node = json_find_first_label(node, name);
	if (node == NULL || node->child == NULL)
	{
		return;
	}
	CArrayInit(guns, sizeof(const GunDescription *));
	for (json_t *gun = node->child->child; gun; gun = gun->next)
	{
		const GunDescription *g = StrGunDescription(gun->text);
		CArrayPushBack(guns, &g);
	}
}
void LoadColor(color_t *c, json_t *node, const char *name)
{
	if (json_find_first_label(node, name) == NULL)
	{
		return;
	}
	if (!TryLoadValue(&node, name))
	{
		return;
	}
	*c = StrColor(node->text);
}
*/
yajl_val JSONFindNode(yajl_val node, const char *path)
{
	// max 256 levels
	const char *pathSplit[256];
	memset(pathSplit, 0, sizeof pathSplit);
	char *pathCopy = strdup(path);
	char *pch = strtok(pathCopy, "/");
	int i = 0;
	yajl_val out = NULL;
	while (pch != NULL)
	{
		if (i == 256)
		{
			fprintf(stderr, "JSON path too long: '%s'\n", path);
			goto bail;
		}
		pathSplit[i] = pch;
		i++;
		pch = strtok(NULL, "/");
	}
	out = yajl_tree_get(node, pathSplit, yajl_t_any);

bail:
	free(pathCopy);
	return out;
}
