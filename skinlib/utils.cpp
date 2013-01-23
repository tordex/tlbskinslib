#include "TxSkinEngine.h"
#include "utils.h"

void trimSpaced(LPWSTR dst, LPWSTR src, int count)
{
	int j = 0;
	for(int i=0; i < count; i++)
	{
		if(j || !j && !_istspace(src[i]))
		{
			dst[j++] = src[i];
		}
	}
	dst[j] = 0;
	j--;
	while(j >= 0 && _istspace(dst[j]))
	{
		dst[j] = 0;
	}
}

LPWSTR findParam(LPWSTR src, LPWSTR param)
{
	if(!src)	return NULL;
	LPWSTR nextParam = StrStr(src, L",");
	if(nextParam)
	{
		trimSpaced(param, src, (int) (nextParam - src));
		param[nextParam - src] = 0;
		return nextParam + 1;
	}
	trimSpaced(param, src, lstrlen(src));
	return src + lstrlen(src);
}

LPWSTR findFunction(LPWSTR src, LPWSTR funcText, LPWSTR params)
{
	if(!src)	return NULL;
	LPWSTR beginParams = StrStr(src, L"(");
	if(!beginParams)
	{
		return NULL;
	}
	LPWSTR endParams = StrStr(beginParams, L")");
	if(!endParams)
	{
		return NULL;
	}
	lstrcpyn(params, beginParams + 1, (int) (endParams - beginParams));
	params[endParams - beginParams] = 0;

	beginParams--;
	while(beginParams != src && _istspace(beginParams[0])) beginParams--;
	if(_istspace(beginParams[0]))
	{
		return NULL;
	}

	LPWSTR beginFunc = beginParams;
	while(beginFunc != src && !_istspace(beginFunc[0])) beginFunc--;
	if(_istspace(beginFunc[0]))
	{
		beginFunc++;
	}
	lstrcpyn(funcText, beginFunc, (int) (beginParams - beginFunc + 2));
	funcText[beginParams - beginFunc + 2] = 0;

	return endParams + 1;
}


void TxSkin::enumSkins( IEnumSkinsCallBack* callback, LPCWSTR path, LPCWSTR iniFile )
{
	TCHAR pathBase[MAX_PATH];
	lstrcpy(pathBase, path);
	PathAddBackslash(pathBase);

	WIN32_FIND_DATA fd;
	TCHAR findMask[MAX_PATH];
	lstrcpy(findMask, path);
	lstrcat(findMask, TEXT("*"));

	HANDLE hFind = FindFirstFile(findMask, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if(lstrcmp(fd.cFileName, TEXT(".")) && lstrcmp(fd.cFileName, TEXT("..")) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TCHAR shortPath[255];
				TCHAR skinFile[MAX_PATH];
				lstrcpy(skinFile, pathBase);
				lstrcat(skinFile, fd.cFileName);
				PathAddBackslash(skinFile);
				lstrcat(skinFile, iniFile);

				lstrcpy(shortPath, fd.cFileName);
				PathAddBackslash(shortPath);
				lstrcat(shortPath, iniFile);

				callback->onSkinFileFound(skinFile, shortPath);
			}
		} 
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}

TxSkin::brush* TxSkin::parseBrush( LPWSTR section, LPWSTR key, LPWSTR iniFile )
{
	WCHAR str[255];
	GetPrivateProfileString(section, key, L"", str, 255, iniFile);
	return parseBrush(str);
}

RECT TxSkin::parseRect( LPWSTR section, LPWSTR key, LPWSTR iniFile )
{
	WCHAR str[255];
	GetPrivateProfileString(section, key, L"0 0 0 0", str, 255, iniFile);
	return parseRect(str);
}

RECT TxSkin::parseRect( LPWSTR str )
{
	DWORD left = 0;
	DWORD top = 0;
	DWORD right = 0;
	DWORD bottom = 255;
	TCHAR strVal[50];
	LPTSTR curPos = str;
	while(curPos[0] && !_istdigit(curPos[0])) curPos++;
	int i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	left = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	top = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	right = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	if(strVal[0])
	{
		bottom = _ttoi(strVal);
	}
	RECT ret;
	ret.left	= left;
	ret.right	= right;
	ret.bottom	= bottom;
	ret.top		= top;

	return ret;
}

TxSkin::brush* TxSkin::parseBrush( LPWSTR str )
{
	WCHAR fncName[255]	= L"";
	WCHAR params[255]	= L"";
	LPWSTR fnc = findFunction(str, fncName, params);
	while(fnc)
	{
		if(!StrCmpI(fncName, L"solidBrush"))
		{
			WCHAR strColor[255]	= L"";
			::findParam(params, strColor);

			TxSkin::color	brushColor	= parseColor(strColor);
			return new TxSkin::solid_brush(brushColor);
		}
		if(!StrCmpI(fncName, L"xGradient"))
		{
			WCHAR strColor1[255]	= L"";
			WCHAR strColor2[255]	= L"";
			LPWSTR nextParam = ::findParam(params, strColor1);
			nextParam = ::findParam(nextParam, strColor2);

			TxSkin::color	brushColor1	= parseColor(strColor1);
			TxSkin::color	brushColor2	= parseColor(strColor2);
			return new TxSkin::h_gradient_brush(brushColor1, brushColor2);
		}
		if(!StrCmpI(fncName, L"yGradient"))
		{
			WCHAR strColor1[255]	= L"";
			WCHAR strColor2[255]	= L"";
			LPWSTR nextParam = ::findParam(params, strColor1);
			nextParam = ::findParam(nextParam, strColor2);

			TxSkin::color	brushColor1	= parseColor(strColor1);
			TxSkin::color	brushColor2	= parseColor(strColor2);
			return new TxSkin::v_gradient_brush(brushColor1, brushColor2);
		}
		if(!StrCmpI(fncName, L"radialGradient"))
		{
			WCHAR strColor1[255]	= L"";
			WCHAR strColor2[255]	= L"";
			LPWSTR nextParam = ::findParam(params, strColor1);
			nextParam = ::findParam(nextParam, strColor2);

			TxSkin::color	brushColor1	= parseColor(strColor1);
			TxSkin::color	brushColor2	= parseColor(strColor2);
			return new TxSkin::radial_gradient_brush(brushColor1, brushColor2);
		}
		fnc = findFunction(fnc, fncName, params);
	}
	return NULL;
}

TxSkin::pen* TxSkin::parsePen( LPWSTR section, LPWSTR key, LPWSTR iniFile )
{
	WCHAR str[255];
	GetPrivateProfileString(section, key, L"", str, 255, iniFile);
	return parsePen(str);
}

TxSkin::pen* TxSkin::parsePen( LPWSTR str )
{
	WCHAR fncName[255]	= L"";
	WCHAR params[255]	= L"";
	LPWSTR fnc = findFunction(str, fncName, params);
	while(fnc)
	{
		if(!StrCmpI(fncName, L"solidPen"))
		{
			WCHAR strColor[255]	= L"";
			WCHAR strSize[255]	= L"";
			LPWSTR nextParam = ::findParam(params, strColor);
			nextParam = ::findParam(nextParam, strSize);

			TxSkin::color	penColor	= parseColor(strColor);
			int				penSize		= _tstoi(strSize);
			return new TxSkin::solid_pen(penColor, penSize);
		}
		fnc = findFunction(fnc, fncName, params);
	}
	return NULL;
}

TxSkin::color TxSkin::parseColor( LPCWSTR section, LPCWSTR key, TxSkin::color defColor, LPCWSTR iniFile )
{
	WCHAR str[255];
	GetPrivateProfileString(section, key, L"", str, 255, iniFile);
	if(!str[0])
	{
		return defColor;
	}
	return parseColor(str);
}

TxSkin::color TxSkin::parseColor( LPWSTR str )
{
	DWORD r = 0;
	DWORD g = 0;
	DWORD b = 0;
	DWORD a = 255;
	TCHAR strVal[50];
	LPTSTR curPos = str;
	while(curPos[0] && !_istdigit(curPos[0])) curPos++;
	int i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	r = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	g = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	b = _ttoi(strVal);

	while(curPos[0] && !_istdigit(curPos[0])) curPos++;

	i=0;
	while(curPos[0] && _istdigit(curPos[0])) 
	{
		strVal[i] = curPos[0];
		i++;
		curPos++;
	}
	strVal[i] = 0;
	if(strVal[0])
	{
		a = _ttoi(strVal);
	}

	return TxSkin::color((BYTE) r, (BYTE) g, (BYTE) b, (BYTE) a);
}
