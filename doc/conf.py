"""
Configuration file for the Sphinx documentation builder, this file expects Doxygen to have been executed first and
its XML output to be at the specified location. Relevant links for documentation of Sphinx and Doxygen are shown below:

  - Sphinx:
    - https://www.sphinx-doc.org/en/master/usage/configuration.html
    - Conditional Documentation: https://stackoverflow.com/a/45637280/21951997
  - Doxygen:
    - Grouping: https://www.doxygen.nl/manual/grouping.html
    - Autolink to enumerations, functions and so on: https://www.doxygen.nl/manual/autolink.html
    - Conditional documentation: https://www.doxygen.nl/manual/commands.html#cmdif
"""

# pylint: skip-file
# type: ignore

import sphinx
import os
import xml.etree.ElementTree as ET

## Project information #################################################################################################
# Project name.
project = "cb"
# Friendly name.
friendly_name = "Circular Buffer"
# Project author.
author = "Diego Martínez García <dmg0345@gmail.com>"
# Project copyright.
copyright = author
# Project version.
version = "1.0.0"
# Project release, set it as the same value as version as the separation is not required.
release = version

## Sphinx General configuration ########################################################################################
# Sphinx and custom extensions.
extensions = [
    # Read The Docs theme extension:
    #    - https://github.com/readthedocs/sphinx_rtd_theme
    #    - https://sphinx-rtd-theme.readthedocs.io/en/stable/
    "sphinx_rtd_theme",
    # Breathe extension:
    #    - https://github.com/breathe-doc/breathe
    #    - https://breathe.readthedocs.io/en/latest/
    #    - https://www.doxygen.nl/manual
    "breathe"
]
# File extensions of source files.
source_suffix = {".rst": "restructuredtext"}
# Encoding of reST files.
source_encoding = "utf-8-sig"
# Root document.
root_doc = "index"
# Glob-style patterns to exclude when looking for source files.
exclude_patterns = [".sphinx_build", ".doxygen_build"]
# Included at the end of every RestructuredText source file that is read.
rst_epilog = f""""""
# Included at the beginning of every RestructuredText source file that is read.
rst_prolog = f"""
.. |ProjectName| replace:: **{project}**
.. |ProjectFriendlyName| replace:: **{friendly_name}**
.. |ProjectCopyright| replace:: **{copyright}**
.. |ProjectVersion| replace:: **{version}**
"""
# Enable nitpicky mode.
nitpicky = True
# Unset pygments style so that the one of the default theme is used for highlighting.
pygments_style = None
# Keep module names hidden in the documentation as that results in too verbose documentation.
add_module_names = False
# Do not add objects such as classes, functions or methods as part of the TOC.
toc_object_entries = False

## Sphinx Internalization configuration ################################################################################
# Set language to english.
language = "en"

## Sphinx HTML output configuration ####################################################################################
# The theme to use.
html_theme = "sphinx_rtd_theme"
# Specific options for the theme.
html_theme_options = {
    "logo_only": False,
    "display_version": True,
    "style_external_links": False,
    "collapse_navigation": False,
    "includehidden": True,
    "navigation_depth": 4,
}
# HTML title.
html_title = f"{friendly_name} v{version}"
# HTML short title.
html_short_title = html_title
# HTML base URL which points to the root HTML documentation.
html_baseurl = ""
# Path to the HTML logo.
html_logo = ".assets/img/logo_128x128.png"
# Path to the HTML fav icon.
html_favicon = ".assets/img/logo_16x16.png"
# Add permalinks to every section.
html_permalinks = True
# Do not include the original sources in the final documentation.
html_copy_source = False
# Do not include links to the sources in the documentation.
html_show_sourcelink = False
# Show copyright in the HTML footer.
html_show_copyright = True
# Disable created by Sphinx text in the HTML footer.
html_show_sphinx = False
# Set the language for the search to english.
html_search_language = "en"

## Breathe Extension configuration #####################################################################################
# Set list of projects and their paths to the XML doxygen files.
breathe_projects = {
    "all": ".doxygen_build/xml"
}
# Set the default project from the projects listed above.
breathe_default_project = "all"
# Set domains by extension for C and C++ separately.
breathe_domain_by_extension = {
    "h": "c",
    "c": "c",
    "hpp": "cpp",
    "cpp": "cpp"
}
# For defines, do not show their values in the documentation.
breathe_show_define_initializer = False
# For enumeration values, do not show their values in the documentation.
breathe_show_enumvalue_initializer = False
# Do not show includes in the documentation.
breathe_show_include = False
# Debug directives.
breathe_debug_trace_directives = True

def breathe_load_tags_on_doxyfile() -> None:
    """Loads the ENABLED_SECTIONS configuration of the DoxyFile when the XML is generated, and adds it as tags, suitable
    to be used with the @only directive in Sphinx.

    Note that only sections with valid Python identifiers are loaded: Only contains alphanumeric letters (a-z) and
    (0-9), or underscores (_). A valid identifier cannot start with a number, or contain any spaces."""
    # Create path to the file and ensure it exists.
    doxyfile_xml = os.path.join(os.path.dirname(os.path.abspath(__file__)), ".doxygen_build", "xml", "Doxyfile.xml")
    if not os.path.exists(doxyfile_xml):
        raise sphinx.errors.ConfigError(f"Could not find Doxyfile at '{doxyfile_xml}'...")
    # Parse XML and get the ENABLED_SECTIONS values, if they exist.
    elems = [elem for elem in ET.parse(doxyfile_xml).getroot() if elem.get("id", "") == "ENABLED_SECTIONS"]
    if len(elems) > 1:
        raise sphinx.errors.ConfigError(f"Multiple 'ENABLED_SECTIONS' XML elements found, check the Doxyfile.")
    # Add all tags with valid Python identifiers.
    if len(elems) == 1:
        _ = [tags.add(tag.text) for tag in elems[0] if tag.text.isidentifier()]

## Setup functionality #################################################################################################
def on_missing_reference(_app: sphinx.application.Sphinx,
                         _env: app.builder.env,
                         node: sphinx.addnodes.pending_xref,
                         contnode: sphinx.addnodes.pending_xref):
    """Handler for 'on_missing_reference' warning. Lots of false positives can be raised by Sphinx from this, for
    example from missing references to standard library types or missing references to third-party types that are not
    documented by Sphinx or which are not processed by Doxygen.

    One solution is to disable errors as warnings, but this is not convenient as it could 

    The implementation of this handler here allows to ignore warnings for the types specified."""
    # Print missing reference in color in the terminal to differentiate it.
    # print(f"\033[93m mon_missing_reference: {node}.\033[00m")

    # Missing references handler, lots of false positives can occur in the C/C++ domain.
    # Trying to handle them with 'c_extra_keywords' or 'c_id_attributes' generates other kind of errors.
    # Allowed reference domains, reference targets and reference types that can be missing are described below.
    refdomains = ["c", "cpp"]
    reftypes = ["identifier"]
    
    # C Standard library identifiers to ignore.
    c_std_reftargets = [
        "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t", "size_t",
        "atomic_size_t"
    ]

    # Build total reftargets.
    reftargets = [
        *c_std_reftargets,
    ]

    # Check if the reference is of C/C++ type and it can be allowed to be missing.
    if all((node["refdomain"] in refdomains, node["reftarget"] in reftargets, node["reftype"] in reftypes)):
        # Return OK in Sphinx.
        return contnode
    # Raise error in Sphinx.
    return None

def setup(app: sphinx.application.Sphinx):
    """Setup function for Sphinx, refer to the links below for details:

        - https://www.sphinx-doc.org/en/master/extdev/appapi.html#module-sphinx.application
        - https://www.sphinx-doc.org/en/master/extdev/appapi.html#sphinx-core-events"""
    # Load relevant tags to the 'tags' object from the Doxyfile.
    breathe_load_tags_on_doxyfile()
    # Register handler for the 'missing-reference' event.
    app.connect("missing-reference", on_missing_reference)
    # Print tags just for informational purpopses.
    print(f"Tags: {[i for i in tags]}.")
