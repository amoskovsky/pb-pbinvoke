pushd %~dp0

@rem set MS VC9 build environment
@call vcv9

devenv libPBInvoke.sln /build Release /project libPBInvoke

popd