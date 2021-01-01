if (NOT STATE_MACHINE_DOCS)
	return()
endif()

find_package(Doxygen REQUIRED)
set(DOXYGEN_GENERATE_TREEVIEW YES)
doxygen_add_docs(state-machine-docs
	${PROJECT_SOURCE_DIR}/src/
	${PROJECT_SOURCE_DIR}/test/
	)
