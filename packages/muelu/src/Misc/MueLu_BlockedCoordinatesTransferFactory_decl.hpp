// @HEADER
//
// ***********************************************************************
//
//        MueLu: A package for multigrid based preconditioning
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#ifndef MUELU_BLOCKEDCOORDINATESTRANSFER_FACTORY_DECL_HPP
#define MUELU_BLOCKEDCOORDINATESTRANSFER_FACTORY_DECL_HPP

#include "MueLu_ConfigDefs.hpp"
#include "MueLu_TwoLevelFactoryBase.hpp"
#include "Xpetra_MultiVector_fwd.hpp"
#include "Xpetra_Matrix_fwd.hpp"

#include "MueLu_BlockedCoordinatesTransferFactory_fwd.hpp"

namespace MueLu {

/*!
  @class BlockedCoordinatesTransferFactory class.
  @brief Class for transferring coordinates from a finer level to a coarser one for BlockedCrsMatrices.
  This basically combines the Coordinates generated by each separate block

  ## Input/output of BlockedCoordinatesTransferFactory ##

  ### User parameters of BlockedCoordinatesTransferFactory ###
  Parameter | type | default | master.xml | validated | requested | description
  ----------|------|---------|:----------:|:---------:|:---------:|------------
  | BlockedCoordinates| Factory | null |   | * | (*) | Factory providing coordinates
  | Aggregates | Factory | null |   | * | (*) | Factory providing aggregates
  | CoarseMap  | Factory | null |   | * | (*) | Generating factory of the coarse map

  The * in the @c master.xml column denotes that the parameter is defined in the @c master.xml file.<br>
  The * in the @c validated column means that the parameter is declared in the list of valid input parameters (see BlockedCoordinatesTransferFactory::GetValidParameters).<br>
  The * in the @c requested column states that the data is requested as input with all dependencies (see BlockedCoordinatesTransferFactory::DeclareInput).

  The BlockedCoordinatesTransferFact first checks whether there is already valid coarse coordinates information
  available on the coarse level. If that is the case, we can skip the coordinate transfer and just reuse
  the available information.
  Otherwise we try to build coarse grid coordinates by using the information from the sub-factories.

  ### Variables provided by BlockedCoordinatesTransferFactory ###

  After BlockedCoordinatesTransferFactory::Build the following data is available (if requested)

  Parameter | generated by | description
  ----------|--------------|------------
  | Coordinates | BlockedCoordinatesTransferFactory   | coarse level coordinates (unified)
*/
  template <class Scalar = DefaultScalar,
            class LocalOrdinal = DefaultLocalOrdinal,
            class GlobalOrdinal = DefaultGlobalOrdinal,
            class Node = DefaultNode>
  class BlockedCoordinatesTransferFactory : public TwoLevelFactoryBase {
#undef MUELU_BLOCKEDCOORDINATESTRANSFERFACTORY_SHORT
#include "MueLu_UseShortNames.hpp"

  public:
    //! @name Constructors/Destructors.
    //@{

    /*! @brief Constructor.

       @param vectorName The name of the quantity to be restricted.
       @param restrictionName The name of the restriction Matrix.

       The operator associated with <tt>projectionName</tt> will be applied to the MultiVector associated with
       <tt>vectorName</tt>.
    */
    BlockedCoordinatesTransferFactory() { }

    //! Destructor.
    virtual ~BlockedCoordinatesTransferFactory() { }

    RCP<const ParameterList> GetValidParameterList() const;

    //@}

    //! @name Input
    //@{

    /*! @brief Specifies the data that this class needs, and the factories that generate that data.

        If the Build method of this class requires some data, but the generating factory is not specified in DeclareInput, then this class
        will fall back to the settings in FactoryManager.
    */
    void DeclareInput(Level &finelevel, Level &coarseLevel) const;

    //@}

    //! @name Build methods.
    //@{

    //! Build an object with this factory.
    void Build(Level & fineLevel, Level &coarseLevel) const;

    //@}

    //@{
    /*! @brief Add (sub) coords factory in the end of list of factories in BlockedCoordinatesTransferFactory.

    */
    void AddFactory(const RCP<const FactoryBase>& factory);


    //! Returns number of sub factories.
    size_t NumFactories() const { return subFactories_.size(); }

    //@}
  private:
    //! list of user-defined sub Factories
    std::vector<RCP<const FactoryBase> > subFactories_;

  }; // class BlockedCoordinatesTransferFactory

} // namespace MueLu

#define MUELU_BLOCKEDCOORDINATESTRANSFERFACTORY_SHORT
#endif // MUELU_BLOCKEDCOORDINATESTRANSFER_FACTORY_DECL_HPP
