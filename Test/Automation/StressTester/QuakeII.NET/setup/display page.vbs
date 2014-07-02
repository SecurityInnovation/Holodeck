
path = Session.Property("CustomActionData") & "setup\finish setup.html"

set o = CreateObject("wscript.shell")
o.Run """" & path & """"
