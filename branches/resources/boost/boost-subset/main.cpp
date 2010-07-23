// Note that any boost headers referenced in this sln should be added to boost-subset
//			(maybe later on, we'll remove all the redundant headers from svn)


#include <boost/thread/detail/tss_hooks.hpp>

// Implementation of function prototype declared in boost/thread/detail/tss_hooks.hpp
// Essentially a hack to make this damn thing link properly
void tss_cleanup_implemented(void)
{
}