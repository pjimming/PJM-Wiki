window.MathJax = {
    tex: {
        inlineMath: [["\\(", "\\)"]],
        displayMath: [["\\[", "\\]"]],
        processEscapes: true,
        processEnvironments: true
    },
    options: {
        ignoreHtmlClass: ".*|",
        processHtmlClass: "arithmatex"
    }
};

document$.subscribe(() => {
    MathJax.typesetPromise()
})

window.MathJax = {
    loader: { load: ['[tex]/color'] },
    tex: { packages: { '[+]': ['color'] } }
};

window.MathJax = {
    loader: { load: ['[tex]/boldsymbol'] },
    tex: { packages: { '[+]': ['boldsymbol'] } }
};

window.MathJax = {
    tex: {
        macros: {
            RR: "{\\bf R}",
            bold: ["{\\bf #1}", 1]
        },
        environments: {
            braced: ["\\left\\{", "\\right\\}"]
        }
    }
};

window.MathJax = {
    loader: { load: ['[tex]/configmacros'] },
    tex: { packages: { '[+]': ['configmacros'] } }
};

window.MathJax = {
    loader: { load: ['[tex]/mathtools'] },
    tex: { packages: { '[+]': ['mathtools'] } }
};

window.MathJax = {
    loader: { load: ['[tex]/textmacros'] },
    tex: { packages: { '[+]': ['textmacros'] } }
};

