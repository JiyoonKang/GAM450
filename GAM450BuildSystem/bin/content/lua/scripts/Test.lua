function Initialize()
  print("Initialized!")
  Events.Subscribe(this, Events.LogicUpdate, Update)
end

function Update(event)
  print(event.MessageId == Events.LogicUpdate)
end

function Shutdown()
  print("ByeBye!")
  Events.Unsubscribe(this, Events.LogicUpdate, Update)
end