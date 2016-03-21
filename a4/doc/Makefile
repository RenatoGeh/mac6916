f_exs = exercises
f_doc = documentation
e_compile = pdflatex -shell-escape $(f_exs).tex
d_compile = pdflatex -shell-escape $(f_doc).tex


all: exs doc

exs:
	make clean; \
	$(e_compile); $(e_compile); \
	biber $(f_exs); \
	$(e_compile); $(e_compile) && \
	zathura $(f_exs).pdf

doc:
	make clean; \
	$(d_compile); $(d_compile); \
	biber $(f_doc); \
	$(d_compile); $(d_compile) && \
	zathura $(f_doc).pdf


.PHONY: clean
clean:
	rm *.aux *.bbl *.bcf *.blg *.log *.out *.run.xml *.toc;
	rm -rf _minted-report

