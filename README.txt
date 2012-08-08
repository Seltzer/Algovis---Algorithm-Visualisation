Algovis was developed by Nathan Pitman and David Olsen as our final year university project. One day, one or both of us will pick up where we left off :)



**************************************** Algovis README *******************************************




1.0 Introduction

This README provides a brief overview of the Algovis framework and points to other documentation sources. There are different possible setups based on what you want to do with Algovis.

It is possible to have your code visualised by writing a program for your Windows compiler of choice and linking it against the algovis datasource. However, this setup is quite involved, so the Algovis team recommends using the provided visual studio solution described below. This eliminates specifying include locations and library dependencies.

The rest of this README assumes that you intend to use the Algovis Visual Studio solution for your development, whether you are simply writing code to be visualised by Algovis, or actually augmenting the Algovis framework.



2.0 Installation Instructions for Algovis (using the Visual Studio solution)

Most of the Algovis dependencies are included within our distribution, meaning that no extra work is required during installation, but there is one external dependency (the QT GUI library). QT must be installed only if you are extending the Algovis framework. In order to simply visualise your code, it is sufficient to copy the QT DLLs to the executable directory.


2.1 QT Installation Instructions

Step 1: Install dependencies\qt-sdk-win-opensource-2010.04.exe. This takes ages.
Step 2: Assuming your installation directory is "C:\Program Files\Qt", add C:\Program Files\Qt\2010.04\bin to your Windows path 
Step 3: Run C:\Program Files\Qt\2010.04\qt\configure.exe from Visual Studio command prompt (this is not the same as Windows command prompt)
	This takes even longer than Step 1. Don't be disheartened if there are errors near the end (this is normal)
Step 4: Install dependencies\qt-vs-addin-1.1.5.exe. This is a Visual Studio QT plugin.


2.2 Algovis Visual Studio Solution Installation 

There is no installation required. Simply open branches\datasource\datasource-wrappers\algovis c++.sln.




3.0 Overview of the Visual Studio Solution


The Algovis VS solution consists of 4 main projects and various internal dependencies (under Resources). These are:
	algovis c++ 	       = Our C++ data source
	algovis c++ unit-tests = Unit-tests for algovis c++
	view		       = The Algovis View module
	algovis test	       = Where we write code which uses Algovis
	Resources\projects     = Various internal dependencies including Boost, CxxTest, SFML and our own utilities library

In order to have your code visualised, write it inside algovis test\algovis test.cpp and then build only the algovis test project (right click algovis-test
and select build). Don't attempt to build any other components (i.e. the Algovis data source or view) unless QT is installed.

Algovis test.cpp is our exemplar user code. As can be seen, in order for it to use Algovis, it need only include the Algovis header (conversions.h) and be linked against Algovis (which the VS solution does implicitly).

We have provided some demos as an example of how to use Algovis. The most relevant of these are ScreenDemo and RecursiveMergeDemo. See algovis test.cpp for more details.



4.0 Feature Set

The feature of Algovis can be divided into 3 categories: the automatic visualisation of supported data structures, the API we offer for performing various tasks in user code and user-interface functions.


4.1 Data Structures

Currently, we support visualisation of the data structures below. When a data structure is instantiated, by default, it is automatically displayed on screen on its own row (unless it is a primitive type - more on this later). Any operations performed on the data structure will also be animated on screen.

Supported Data Structures:
* std::vector (a dynamic array)
* std::deque (a double-ended queue)
* int/char/float/double primitive types
* A square matrix class called AlgoSquareMatrix


4.2 User-Interface

The View module of Algovis is responsible for displaying supported data structures and allowing the user to manipulate the animation produced. The user interface is a bit rough around the edges as our focus was on providing a proof-of-concept prototype. UI features include:

* Moving data structures around on screen through mouse dragging
* Labelling data structures by means of a right click context menu
* Zooming in and out on data structures using mouse scrolling
* Pausing/resuming animations using the Play/Pause button marked |> ||
* Skipping the current animation by using the Next button marked |>
* Backtracking/forwardtracking through animations using the buttons marked |<< and >>| (this is somewhat broken at the moment)
* Toggling animations on and off using the Toggle Animations button.


4.3 User Functions

While Algovis attempts to automatically layout data structures in a meaningful manner, it does allow the user to control the layout either by using UI functions mentioned in 4.2 or by calling helper functions provided in "algovis c++/userfunctions.h". We now provide a brief overview of these functions:

* EnableCommunicationWithView
This should be called at the point after which user code should be visualised.

* EnableOperandsHighlighting
We provide an animation for the comparison of primitive types using comparison operators such as < <= > >= == !=. This type of animation is enabled by default and is useful for visualising sorting algorithms. But it can be disabled using this function.

* PlaceNextOnSameLineAsLast
This ensures that the next data structure to be instantiated, will be placed on the same row as the last data structure which was instantiated.

* PlaceNextTwoOnSameLine
This ensures that the next two data structures to be instantiated, will be placed on the same row as each other.

* EnsureThisIsDisplayed
By default, primitive types are not displayed. After instantiating a primitive, the user can enforce its display by calling this function.

* SetCaption
This allows the user to set the caption in the bottom panel of the View.

* FlushAllActions
This should be called at the end of user code.


4.4 Miscellaneous

Some external code (i.e. libraries) do not like Algovis' redefinition of core C++ datatypes. A header unconversions.h is provided to remove these redefinitions, but extreme care must be taken to ensure correct include order. This is not recommended for general users.



5.0 Additional Resources

This README is definitely not comprehensive enough to cover everything and it is recommended that the reader refer to the resources outlined below in order to gain a better understanding of Algovis:


* Project Poster - provided in docs\P4P poster.pdf
* Conference Slides - provided in docs
* Interim and Final Reports - provided in docs\Interim & Final Reports
* Algovis TODO - provided in branches\datasource\datasource-wrappers\TODO.txt and easily accessible from inside the Visual Studio solution


Scope for future work is detailed in the final reports and to an extent, in the TODO. 












For detailed developer documentation aimed at those wanting to further develop the Algovis framework (by improving the View, improving the C++ data source, or adding another data source), see docs\dev docs.txt


