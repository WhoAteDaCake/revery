/* Reconciler.re
 *
 * This implements a reconciler for our UI primitives
 */

open RenderPass;

type node = Node.node(renderPass);

type primitives =
  | View(Style.t, NodeEvents.t(node), option(int))
  | Text(Style.t, string, NodeEvents.t(node))
  | Image(Style.t, string, NodeEvents.t(node));

let createInstance = prim => {
  let node =
    switch (prim) {
    | View(style, events, tabindex) =>
      let view = (new ViewNode.viewNode)();
      view#setStyle(style);
      view#setEvents(events);
      view#setTabIndex(tabindex);
      view;
    | Image(style, src, events) =>
      let img = (new ImageNode.imageNode)(src);
      img#setStyle(style);
      img#setEvents(events);
      img;
    | Text(style, text, events) =>
      let text = (new TextNode.textNode)(text);
      text#setStyle(style);
      text#setEvents(events);
      (text :> node);
    };

  node;
};

let updateInstance = (n: node, _oldPrim: primitives, newPrim: primitives) =>
  switch (newPrim) {
  | View(style, events, tabindex) =>
    /* TODO: Is there a way to downcast properly here, from Node -> ViewNode ? */
    let vn: ViewNode.viewNode = Obj.magic(n);
    vn#setStyle(style);
    vn#setEvents(events);
    vn#setTabIndex(tabindex);
  | Text(style, text, events) =>
    /* TODO: Is there a way to downcast properly here, from Node -> TextNode ? */
    let tn: TextNode.textNode = Obj.magic(n);
    tn#setStyle(style);
    tn#setText(text);
    tn#setEvents(events);
  | Image(style, _src, events) =>
    let imageNode: ImageNode.imageNode = Obj.magic(n);
    imageNode#setStyle(style);
    imageNode#setEvents(events);
  };

let appendChild = (parent: node, child: node) => parent#addChild(child);

let removeChild = (parent: node, child: node) => parent#removeChild(child);

let replaceChild = (parent: node, newChild: node, oldChild: node) => {
  removeChild(parent, oldChild);
  appendChild(parent, newChild);
  ();
};