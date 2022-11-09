
set( pkginclude_HEADERS atomic_int.h
                        keywords.h
        		memory.h
        		noncopyable.h
			thread_local.h
        		type_traits.h )

# notice that defs.h is not referenced here
INSTALL (FILES ${pkginclude_HEADERS}  
         DESTINATION include/CLHEP/Utility )

