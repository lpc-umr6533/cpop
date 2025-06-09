#ifndef Writable_HH
#define Writable_HH

#include <QString>
#include <QXmlStreamWriter>

/// \brief define function needed to export information and import it.
/// @author Henri Payno
class Writable {
public:
	virtual ~Writable() = default;

	/// \brief  export all information needed to sumarize the Writable
	virtual	void write(QXmlStreamWriter& writer) const = 0;
};

#endif
