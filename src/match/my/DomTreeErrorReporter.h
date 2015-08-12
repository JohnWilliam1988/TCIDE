#if !defined(DOMTREEERRORREPORTER_H)
#define DOMTREEERRORREPORTER_H

#include <match/util/MatchDefs.hpp>
#include <match/sax/ErrorHandler.hpp>

MATCH_CPP_NAMESPACE_USE

class XMLPARSER_EXPORT DomTreeErrorReporter : public ErrorHandler
{
public:
	// -----------------------------------------------------------------------
	//  Constructors and Destructor
	// -----------------------------------------------------------------------
	DomTreeErrorReporter() :
	  fSawErrors(false)
	  {
	  }

	  ~DomTreeErrorReporter()
	  {
	  }

	  // -----------------------------------------------------------------------
	  //  Implementation of the error handler interface
	  // -----------------------------------------------------------------------
	  void warning(const SAXParseException& toCatch);
	  void error(const SAXParseException& toCatch);
	  void fatalError(const SAXParseException& toCatch);
	  void resetErrors();

	  // -----------------------------------------------------------------------
	  //  Getter methods
	  // -----------------------------------------------------------------------
	  bool getSawErrors() const;

	  // -----------------------------------------------------------------------
	  //  Private data members
	  //
	  //  fSawErrors
	  //      This is set if we get any errors, and is queryable via a getter
	  //      method. Its used by the main code to suppress output if there are
	  //      errors.
	  // -----------------------------------------------------------------------
	  bool    fSawErrors;
};


inline bool DomTreeErrorReporter::getSawErrors() const
{
    return fSawErrors;
}


#endif
