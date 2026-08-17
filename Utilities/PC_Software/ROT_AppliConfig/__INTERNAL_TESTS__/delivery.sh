#! bash
echo "optional parameter delivery, push or build"
version=$(grep -e "SOURCE_VERSION\s*=\s*\"" ../AppliCfg/main.py|cut -d'"' -f2)
tag=$(git tag|grep -e "^$version$")
out=0
if [ "$tag" != "" ]; then
  echo "version already tagged"
  out=1
else
  echo "version : $1"
fi
rn=$(grep "$version" ../Release_Notes.md)
if [ "$rn" == "" ]; then
  echo "Release note not updated"
  out=1
fi

pytest -s AppliCfg_test_utils.py
if [ $? -eq 0 ] && [ $out -eq 0 ] ; then
	cd ..
	#Success
	pyinstaller --clean -y AppliCfg.py>/dev/null
	pyinstaller AppliCfg.py --onedir -y -n AppliCfg
	if [ $? != 0 ]; then
		cp -f dist/AppliCfg/*.exe dist/*.exe
        ./dist/AppliCfg/AppliCfg.exe --version
		pandoc --self-contained -s -f markdown Release_Notes.md -t html5 -c "_htmresc/mini-st.css" -o Release_Notes.html
		if [ $1 == "delivery" ] && [ $(git branch --show-current) == "master" ]; then
			echo "delivery"
			rm -rf __INTERNAL_TESTS__
			#update of ITSbuilder.exe, Release_Notes.html
			git commit -m "update $version" dist/AppliCfg/*.*
			git commit --amend Release_Notes.html
			git push stm32cube master:master
			git tag $version
			git push stm32cube $version
		elif [ $1 == "push" ] && [ $(git branch --show-current) == "dev" ]; then
			git commit -m "update $version" dist/AppliCfg/*.*
			git commit --amend Release_Notes.html
			git push stm32cube master:iso/dev
		fi
	fi
fi
exit $out
