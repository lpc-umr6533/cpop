#ifndef MESSENGERBASE_HH
#define MESSENGERBASE_HH

#include <memory>

#include "G4UImessenger.hh"

class G4UIdirectory;

namespace cpop {

class MessengerBase : public G4UImessenger
{
public:
    MessengerBase();
    ~MessengerBase() = default;

    virtual void BuildCommands(G4String base) = 0;

    /// \brief Create a directory called base + "/" + suffix + "/" and return it without the last "/".
    G4String make_directory(G4String base, G4String suffix);

private:
    std::unique_ptr<G4UIdirectory> dir_;
};

}

#endif // MESSENGERBASE_HH
