#!/usr/bin/env pwsh

if (!(Test-Path -Path "boost/version.hpp")) {
    if (! (Get-ChildItem -Name | Select-String -CaseSensitive -SimpleMatch -Quiet -Pattern "bootstrap") )
    {
        if (!( Get-Location | Select-String -CaseSensitive -SimpleMatch -Quiet -Pattern "/libs/boost"))
        {
            Set-Variable -Name "SI_HAS_CD_FOR_BOOST"
            Set-Location -Path "./libs/boost"
        }
    }

    if (!(Test-Path -Path "boost/version.hpp"))
    {
        Write-Output "Initializing Boost for Silicon Desktop..."

        ./bootstrap.sh
        ./b2 headers
    }
    else
    {
        Write-Output "Boost for Silicon Desktop found!"
    }
}

if ( "$SI_HAS_CD_FOR_BOOST" )
{
    Set-Location -Path -
}

Remove-Variable SI_HAS_CD_FOR_BOOST