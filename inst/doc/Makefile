LATEXFILES = *.aux\
        *.bbl\
        *.blg\
        *.ilg\
        *.log\
        *.nlo\
        *.nls\
        *.toc\
        *.aux\
        Rdisop.tex\
        Rdisop-isotopes.pdf\
        *.dvi\
        *.map\
        *.out\

all:
	make vignette
	make clean

setvars:
ifeq (${R_HOME},)
R_HOME= $(shell R RHOME)
endif

vignette:
	"$(R_HOME)/bin/R" CMD Sweave Rdisop.Rnw
	pdflatex Rdisop.tex
	bibtex Rdisop
	pdflatex Rdisop.tex
	pdflatex Rdisop.tex

clean:
	rm -f $(LATEXFILES)
	rm -f *~

