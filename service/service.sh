cmd=$1
if [ "$cmd" = "install" ]; then
  dst=$HOME/.config/systemd/user/
  echo Installing to $dst ...
  cp *.service $HOME/.config/systemd/user/ || exit 1
  systemctl --user daemon-reload
  echo done!
else
  echo Executing $cmd ...
  systemctl --user $cmd curves-ws.service
  systemctl --user $cmd curves-doc-doxygen.service
  systemctl --user $cmd curves-doc-sphinx.service
fi
