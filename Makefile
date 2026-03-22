.PHONY: build
build:
		uv run setup.py build_ext --inplace
