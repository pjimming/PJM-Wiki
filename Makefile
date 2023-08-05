upload:
	git add .
	git commit -m "$(msg)"
	git push
	git pull
	mkdocs build --clean
	mkdocs gh-deploy