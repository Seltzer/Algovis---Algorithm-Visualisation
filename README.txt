**************************************** Algovis README *******************************************


1.0 Introduction

This README provides a brief overview of the Algovis framework and points to other documentation sources. There are different possible setups based on what you want to do with Algovis.

It is possible to have your code visualised by writing a program for your Windows compiler of choice and linking it against the algovis datasource. However, this setup is quite involved, so the Algovis team recommends using the provided visual studio solution described below. This eliminates specifying include locations and library dependencies.

The rest of this README assumes that you intend to use the Algovis Visual Studio solution for your development, whether you are simply writing code to be visualised by Algovis, or actually augmenting the Algovis framework.



2.0 Installation Instructions for Algovis (using the Visual Studio solution)

Most of the Algovis dependencies are included within our distribution, meaning that no extra work is required during installation, but there is one external dependency (the QT GUI library). QT must be installed only if you are extending the Algovis framework. In order to simply visualise your code, it is sufficient to copy the QT DLLs to the executable directory.





Extending/developing algovis


general:


== System Requirements ==

Visual studio 2008 (


== Installation ==

Copy the project directory to


system requirementshow to use
sln stuff

A TODO is included in the solution directory (branches\datasource\datasource-wrappers)s


user docs:

feature set - different types of functionality
user functions in code
ui stuff
supported data structures
demos


For detailed developer documentation aimed at those wanting to further develop the Algovis framework (by improving the View, improving the C++ data source, or adding another data source), see docs\dev docs.txt


