# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import os, sphinx_rtd_theme
html_theme = "sphinx_rtd_theme"

html_theme_options = {
    'canonical_url': '',
#    'analytics_id': '',  #  Provided by Google in your dashboard
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    'logo_only': False,
    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False
}

# -- Project information -----------------------------------------------------

project = 'Curves Construction'
copyright = ''
author = ''

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
# extensions = [
#     'sphinx.ext.autodoc',
#     'sphinx.ext.napoleon',
#     'sphinx_rtd_theme',
#     # 'breathe',
# ]

breathe = None
extensions = [
    'sphinx.ext.napoleon',
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
    'sphinx.ext.inheritance_diagram',
    'breathe'
]


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# C++ documentation will be generated into...
doxygen_dir = 'doxygen.output'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'old', doxygen_dir]

highlight_language = 'c++'

# -- Options for HTML output -------------------------------------------------

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess, os

if 1:

    subprocess.call('doxygen doxygen.config', shell=True)

    breathe_projects = {
        "Curves": "doxygen.output/xml/"
    }
    breathe_default_project = "Curves"
    breathe_default_members = ('members', 'undoc-members')
