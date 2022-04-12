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